#pragma once

#include "State.h"
#include "ReadableParams.h"
#include "WritableParams.h"
#include "Arguments.h"
#include "StreamReader.h"
#include "GlobalVars.h"

namespace Lua {

	class Context {
	public:

		typedef Arguments<WritableParams, ReadableParams> CallerArguments;
		typedef Arguments<ReadableParams, WritableParams> CalleeArguments;

		Context() :
			_numOfInputArgs(0),
			_numOfOutputArgs(0)
		{}

		virtual State& state() = 0;
		
		Lua::GlobalVars global(
			const std::string& nameList
		)
		{
			return GlobalVars(state(), nameList);
		}

		CallerArguments args()
		{
			// the caller writes input arguments for the called function
			// and reads output arguments from the called function
			return CallerArguments(
				// function input arguments
				WritableParams{
					state(),
					// gets number of input arguments
					[this] (State& state)
					{
						return _numOfInputArgs;
					},
					// prepares to write the next input argument
					[this] (State& state)
					{
						_prepareInput();
					},
					// finishes writing of the input argument
					[this] (State& state)
					{
						_finishInput();
						_numOfInputArgs++;
					}
				},
				// function output arguments
				ReadableParams{
					state(),
					// gets number of output arguments
					[this] (State& state)
					{
						return _numOfOutputArgs;
					},
					// prepares to read the next output argument
					[this] (State& state)
					{
						if (_numOfOutputArgs == 0) {
							throw std::logic_error("too few arguments");
						}
						_prepareOutput();
					},
					// finishes reading of the output argument
					[this] (State& state)
					{
						_nextOutput();
						_numOfOutputArgs--;
					}
				}
			);
		}
		
		ReadableParams pcall(const std::string& funcName)
		{
			_call();

			// adjust the top of the stack to the number of input arguments
			// it removes unread function input arguments or unread
			// output arguments from the previous function call
			state().setStackTop(_numOfInputArgs);

			_numOfOutputArgs = 0;

			// get the global function and push it at the top of the stack
			state().pushGlobal(funcName);

			// move the function to the beginning of the stack
			state().moveTopElementTo(1);

			// call the function
			state().pcall(_numOfInputArgs);

			_numOfInputArgs = 0;
			_numOfOutputArgs = state().getStackTop();

			return args().out();
		}

		void openStdLibs()
		{
			state().openStdLibs();
		}

		template<typename InputStream>
		void doStream(
			InputStream& is,
			const std::string& chunkName
		)
		{
			StreamReader<InputStream> reader(is);

			state().loadChunk(
				&StreamReader<InputStream>::cb,
				&reader,
				chunkName.c_str()
			);

			state().pcall();
		}

		template<typename T>
		void setGlobal(const std::string& name, T val)
		{
			int numOfArgs = 0;

			WritableParams args(
				state(),
				[&numOfArgs] (State&) { return numOfArgs; },
				[] (State&) {},
				[&numOfArgs] (State&) { numOfArgs++; }
			);

			args << val;

			state().setGlobal(name.c_str());
		}

	protected:

		virtual void _prepareInput()
		{
			// nothing to do here by dafult
		}

		virtual void _finishInput()
		{
			// move a new input argument from the top of the stack
			// to the beginning of the stack right after other
			// input arguments
			state().moveTopElementTo(args().in().count() + 1);
		}

		virtual void _prepareOutput()
		{
			// insert the current output argument at the the top of the stack
			// the current output argument is placed right after input arguments
			state().pushElementFrom(args().in().count() + 1);
		}

		virtual void _nextOutput()
		{
			// remove the current output argument from the stack
			state().removeElementAt(args().in().count() + 1);
		}

		virtual void _call()
		{
			// nothing to do here by dafult
		}

	private:

		int _numOfInputArgs;
		int _numOfOutputArgs;

	};

}