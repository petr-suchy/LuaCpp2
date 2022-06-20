#pragma once

#include "WritableValue.h"

namespace Lua {

	class Closure : public WritableValue {
	public:

		Closure()
		{}

		class End : public WritableValue {
		public:

			End(lua_CFunction func) :
				_func(func)
			{}

			virtual void insertTo(State& state)
			{
				state.leaveClosure();

				lua_pushcclosure(
					state.getL(),
					_func,
					state.getNumOfUpValues()
				);

				state.finishWriting();
			}

		private:

			lua_CFunction _func;

		};

		virtual void insertTo(State& state)
		{
			state.prepareWriting();
			state.enterClosure();
		}

	};

}