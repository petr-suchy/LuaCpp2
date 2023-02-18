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

				_state.open();

				{
					AuxContextFromState temp(_state);
					_initFunc(temp);
				}

			}

			return _state;
		}

	private:

		State _state;
		InitFunc _initFunc;

	};

}