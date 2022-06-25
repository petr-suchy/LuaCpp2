#pragma once

#include "ReadableValue.h"

namespace Lua {

	template<typename T>
	class ReadableNumber : public ReadableValue {
	public:

		typedef std::numeric_limits<T> HostLimits;

		ReadableNumber(T& num) :
			_num(num)
		{}

		virtual void getFrom(State& state)
		{
			state.prepareReading(LUA_TNUMBER);

			lua_Number num = lua_tonumber(
				state.getL(),
				state.getStackTop()
			);

			if (num < (HostLimits::min)()) {
				state.pop();
				throw std::overflow_error(
					"value exceeds host minimum"
				);
			}

			if (num > (HostLimits::max)()) {
				state.pop();
				throw std::overflow_error(
					"value exceeds host maximum"
				);
			}

			_num = (T) num;

			state.finishReading();
		}

	private:

		T& _num;

	};

}