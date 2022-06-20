#pragma once

#include "WritableValue.h"

namespace Lua {

	class WritableNumber : public WritableValue {
	public:

		WritableNumber(const int& num) :
			_num(num)
		{}

		virtual void insertTo(State& state)
		{
			state.prepareWriting();
			lua_pushnumber(state.getL(), _num);
			state.finishWriting();
		}

	private:

		const int& _num;

	};

}