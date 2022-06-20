#pragma once

#include "ReadableValue.h"

namespace Lua {

	class ReadableNumber : public ReadableValue {
	public:

		ReadableNumber(int& num) :
			_num(num)
		{}

		virtual void getFrom(State& state)
		{
			state.prepareReading(LUA_TNUMBER);
			_num = (int) lua_tonumber(state.getL(), state.getStackTop());
			state.finishReading();
		}

	private:

		int& _num;

	};

}