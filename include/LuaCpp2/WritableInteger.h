#pragma once

#include "WritableValue.h"
#include "WritableStackSlot.h"

namespace Lua {

	template<typename T>
	class WritableInteger : public WritableValue {
	public:

		typedef std::numeric_limits<Library::Integer> EngineLimits;

		WritableInteger(T num) :
			_num(num)
		{}

		virtual void insertTo(State& state) const
		{
			WritableStackSlot slot(state);
			
			slot.prepare();
			slot.insertInteger((Library::Integer) _num);
			slot.finish();
		}

	private:

		T _num;

	};

	template<typename T>
	class UnsignedWritableInteger : public WritableValue {
	public:

		typedef std::numeric_limits<Library::Integer> EngineLimits;

		UnsignedWritableInteger(T num) :
			_num(num)
		{}

		virtual void insertTo(State& state) const
		{
			if (_num > (unsigned long long) (EngineLimits::max)()) {
				throw std::overflow_error(
					"value exceeds engine maximum"
				);
			}

			WritableStackSlot slot(state);

			slot.prepare();
			slot.insertInteger((Library::Integer) _num);
			slot.finish();
		}

	private:

		T _num;

	};

}