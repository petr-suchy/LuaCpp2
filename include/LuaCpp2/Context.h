#pragma once

#include "State.h"
#include "ReadableParams.h"
#include "WritableParams.h"
#include "Arguments.h"
#include "StreamReader.h"
#include "GlobalVars.h"
#include "AbstractFunction.h"

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
		
		void openStdLibs()
		{
			state().openStdLibs();
		}

		bool isGlobal(const std::string& name)
		{
			state().getGlobal(name);
			bool result = !state().isNilAt(State::StackTop);
			state().pop();

			return result;
		}

		Lua::GlobalVars global(const std::string& nameList)
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
						_finishOutput();
						_numOfOutputArgs--;
					}
				}
			);
		}
		
		ReadableParams pcall(const std::string& funcName)
		{
			prepareCalling();
			// get a global function with the given name
			// and insert it at the top of the stack
			state().getGlobal(funcName);
			// call that function
			finishCalling();

			return args().out();
		}

		template<typename First, typename... Rest>
		ReadableParams pcall(
			const std::string& funcName,
			const First& first,
			const Rest&... rest
		)
		{
			args().in() << first;
			return pcall(funcName, rest...);
		}
		
		ReadableParams pcall(AbstractFunction& func)
		{
			prepareCalling();
			state() << func;
			finishCalling();

			return args().out();
		}

		template<typename First, typename... Rest>
		ReadableParams pcall(
			AbstractFunction& func,
			const First& first,
			const Rest&... rest
		)
		{
			args().in() << first;
			return pcall(func, rest...);
		}
		
		template<typename InputStream>
		ReadableParams doStream(
			InputStream& is,
			const std::string& chunkName = ""
		)
		{
			StreamReader<InputStream> reader(is);

			prepareCalling();

			state().loadChunk(
				&StreamReader<InputStream>::cb,
				&reader,
				chunkName
			);

			finishCalling();

			return args().out();
		}

		ReadableParams doString(
			const std::string& script = "",
			const std::string& chunkName = ""
		)
		{
			std::stringstream ss(script);
			return doStream(ss, chunkName);
		}
		
	protected:

		virtual void _prepareInput()
		{
			// nothing to do here by dafult
		}

		virtual void _finishInput()
		{
			// nothing to do here by dafult
		}

		virtual void _prepareOutput()
		{
			// insert the current output argument at the the top of the stack
			state().pushValueFrom(1);
		}

		virtual void _finishOutput()
		{
			// remove the current output argument from the stack
			state().removeValueAt(1);
		}

		virtual void _prepareCalling()
		{
			// nothing to do here by dafult
		}

	private:

		int _numOfInputArgs;
		int _numOfOutputArgs;

		void prepareCalling()
		{
			_prepareCalling();

			// remove unread function input arguments or unread
			// output arguments from the previous function call
			state().removeValueAt(1, state().getStackTop() - _numOfInputArgs);

			// adjust the top of the stack to the number of input arguments
			state().setStackTop(_numOfInputArgs);

			_numOfOutputArgs = 0;
		}

		void finishCalling()
		{
			// move a function at the top of the stack to the beginning
			state().moveTopValueTo(1);

			try {
				// call that function
				state().pcall(_numOfInputArgs);
				_numOfInputArgs = 0;
				_numOfOutputArgs = state().getStackTop();
			}
			catch (...) {
				_numOfInputArgs = 0;
				_numOfOutputArgs = 0;
				throw;
			}
		}

	};

	class AuxiliaryContext : public Context {
	public:

		AuxiliaryContext(Library::State* L) :
			_state(L)
		{}

		AuxiliaryContext(State& state) :
			_state(state.getL())
		{}

		virtual State& state()
		{
			return _state;
		}

	private:

		State _state;

	};

}