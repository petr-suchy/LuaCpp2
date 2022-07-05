#pragma once

#include "ReadableValue.h"

namespace Lua {

	class ReadableBoolean : public ReadableValue {
	public:

		ReadableBoolean(bool& val) :
			_val(val)
		{}

		virtual void getFrom(State& state)
		{
			state.prepareReading(LUA_TBOOLEAN);
			_val = lua_toboolean(state.getL(), state.getStackTop()) != 0;
			state.finishReading();
		}

	private:

		bool& _val;

	};

}