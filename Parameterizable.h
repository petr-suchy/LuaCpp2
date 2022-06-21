#pragma once

#include "Context.h"

namespace Lua {

	class Parameterizable {
	public:

		typedef std::function<void(
			Context::CalleeArguments&,
			Context&
		)> Func;

		Parameterizable(Func func) :
			_func(func)
		{}

		int operator() (State& state)
		{
			// number of input and output arguments
			// of the called closure function
			int _numOfFuncInArgs = state.getStackTop();
			int _numOfFuncOutArgs = 0;

			// create auxiliary context used by the called
			// closure function to call nested function
			AuxiliaryContext callee(
				state,
				// called before writing the next input argument
				[&_numOfFuncOutArgs] (Context& ctx)
				{
					// the input argument must be written before writing
					// the output arguments of the called closure function
					if (_numOfFuncOutArgs > 0) {
						throw std::logic_error(
							"output argument after input argument for nested calling"
						);
					}
				},
				// called before calling a function from the closure function
				[&_numOfFuncInArgs, &_numOfFuncOutArgs] (Context& ctx)
				{
					// the output argument of the called closure function
					// must not be written before calling the function
					if (_numOfFuncOutArgs > 0) {
						throw std::logic_error(
							"output argument before calling nested function"
						);
					}
					_numOfFuncInArgs = 0;
				}
			);

			// create arguments for the called closure function
			// the closure function reads input arguments from
			// its caller and writes output arguments for the caller
			Context::CalleeArguments funcArgs(
				// function input arguments
				ReadableParams{
					state,
					// gets number of input arguments
					[&_numOfFuncInArgs] (State& state)
					{
						return _numOfFuncInArgs;
					},
					// prepares to read the next input argument
					[&_numOfFuncInArgs, &_numOfFuncOutArgs](State& state)
					{
						if (_numOfFuncInArgs == 0) {
							throw std::logic_error("too few arguments");
						}
						// insert the current input argument at the the top of the stack
						// the current input argument is placed right after output arguments
						state.pushElementFrom(_numOfFuncOutArgs + 1);
					},
					// finishes reading of the input argument
					[&_numOfFuncInArgs, &_numOfFuncOutArgs](State& state)
					{
						// remove the current input argument from the stack
						state.removeElementAt(_numOfFuncOutArgs + 1);
						_numOfFuncInArgs--;
					}
				},
				// function output arguments
				WritableParams{
					state,
					// get number of output arguments
					[&_numOfFuncOutArgs] (State& state)
					{
						return _numOfFuncOutArgs;
					},
					// prepares to write the next output argument
					[&callee] (State& state)
					{
						// the output argument of the called closure function
						// must not be written if there already are input arguments
						// for nested calling of function
						if (callee.args().in().count() > 0) {
							throw std::logic_error(
								"output argument after input argument for nested calling"
							);
						}
					},
					// finishes writing of the output argument
					[&_numOfFuncOutArgs, &callee] (State& state)
					{
						// move the new output argument from the top of the stack
						// to the beginning of the stack right after other output
						// arguments
						state.moveTopElementTo(_numOfFuncOutArgs + 1);
						_numOfFuncOutArgs++;
					}
				}
			);

			// call the wrapped closure function
			_func(funcArgs, callee);

			return _numOfFuncOutArgs;
		}

	private:

		class AuxiliaryContext : public Context {
		public:

			typedef std::function<void(Context&)> Callback;

			AuxiliaryContext(
				State& state,
				Callback prepareInputFunc,
				Callback prepareCallingFunc
			) :
				_state(state),
				_prepareInputFunc(prepareInputFunc),
				_prepareCallingFunc(prepareCallingFunc)
			{}

		protected:

			virtual State& state()
			{
				return _state;
			}

			virtual void _prepareInput()
			{
				_prepareInputFunc(*this);
				Context::_prepareInput();
			}

			virtual void _prepareCalling()
			{
				_prepareCallingFunc(*this);
				Context::_prepareCalling();
			}

		private:

			State& _state;
			Callback _prepareInputFunc;
			Callback _prepareCallingFunc;

		};

		Func _func;

	};

}