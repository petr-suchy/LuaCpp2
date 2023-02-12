#pragma once

#include "ReadableValue.h"
#include "ReadableStackSlot.h"

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
			ReadableStackSlot slot(state);

			slot.prepare(LUA_TNUMBER);

			auto num = slot.getNumber();

			if (num < (HostLimits::min)()) {
				throw std::overflow_error(
					"value exceeds host minimum"
				);
			}

			if (num > (HostLimits::max)()) {
				throw std::overflow_error(
					"value exceeds host maximum"
				);
			}

			_num = (T) num;
			slot.finish();
		}

	private:

		T& _num;

	};

}