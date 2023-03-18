#pragma once

#include "ReadableValue.h"
#include "ReadableStackSlot.h"

namespace Lua {

	template<typename T>
	class ReadableInteger : public ReadableValue {
	public:

		typedef std::numeric_limits<T> HostLimits;

		ReadableInteger(T& num) :
			_num(num)
		{}

		virtual void getFrom(State& state)
		{
			ReadableStackSlot slot(state);

			slot.prepare(Library::Type::Integer);
			auto num = slot.getInteger();

			if (num < (HostLimits::min)()) {
				state.pop();
				throw std::overflow_error(
					"integer value exceeds host minimum"
				);
			}

			if (num > (HostLimits::max)()) {
				state.pop();
				throw std::overflow_error(
					"integer value exceeds host maximum"
				);
			}

			_num = (T) num;
			slot.finish();
		}

	private:

		T& _num;

	};

	template<typename T>
	class UnsignedReadableInteger : public ReadableValue {
	public:

		typedef std::numeric_limits<T> HostLimits;

		UnsignedReadableInteger(T& num) :
			_num(num)
		{}

		virtual void getFrom(State& state)
		{
			ReadableStackSlot slot(state);

			slot.prepare(Library::Type::Integer);
			auto num = slot.getInteger();

			if (num < 0) {
				state.pop();
				throw std::invalid_argument(
					"unsigned type expected, got negative value"
				);
			}

			if ((unsigned long long) num > (HostLimits::max)()) {
				state.pop();
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