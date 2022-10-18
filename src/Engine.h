#pragma once

#include "Context.h"

namespace Lua {

	class Engine : public Context {
	public:

		typedef std::function<void(Context&)> InitFunc;

		Engine() :
			Engine(
				[] (Context&)
				{}
			)
		{}

		Engine(InitFunc initFunc) :
			_initFunc(initFunc)
		{}

		virtual State& state()
		{
			if (!_state.isOpen()) {

				State newState;

				newState.open();

				if (!newState.isOpen()) {
					throw std::runtime_error("not enough memory");
				}

				AuxContextFromState temp(newState);
				_initFunc(temp);

				newState.setStackTop(0);
				_state.swap(newState);

			}

			return _state;
		}

	private:

		State _state;
		InitFunc _initFunc;

	};

}