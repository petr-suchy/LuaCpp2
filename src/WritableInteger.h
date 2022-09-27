#pragma once

#include "WritableValue.h"
#include "WritableStackSlot.h"

namespace Lua {

	template<typename T>
	class WritableInteger : public WritableValue {
	public:

		typedef std::numeric_limits<lua_Integer> EngineLimits;

		WritableInteger(T num) :
			_num(num)
		{}

		virtual void insertTo(State& state)
		{
			WritableStackSlot slot(state);
			
			slot.prepare();
			slot.insertInteger((lua_Integer) _num);
			slot.finish();
		}

	private:

		T _num;

	};

	template<typename T>
	class UnsignedWritableInteger : public WritableValue {
	public:

		typedef std::numeric_limits<lua_Integer> EngineLimits;

		UnsignedWritableInteger(T num) :
			_num(num)
		{}

		virtual void insertTo(State& state)
		{
			if (_num > (unsigned long long) (EngineLimits::max)()) {
				throw std::overflow_error(
					"value exceeds engine maximum"
				);
			}

			state.prepareWriting();
			lua_pushinteger(state.getL(), (lua_Integer) _num);
			state.finishWriting();
		}

	private:

		T _num;

	};

}