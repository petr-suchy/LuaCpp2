#pragma once

#include "WritableValue.h"

namespace Lua {

	template<typename T>
	class WritableNumber : public WritableValue {
	public:

		typedef std::numeric_limits<lua_Number> EngineLimits;

		WritableNumber(T num) :
			_num(num)
		{}

		virtual void insertTo(State& state)
		{
			state.prepareWriting();
			lua_pushnumber(state.getL(), (lua_Number) _num);
			state.finishWriting();
		}

	private:

		T _num;

	};

}