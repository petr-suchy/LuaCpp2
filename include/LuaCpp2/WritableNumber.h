#pragma once

#include "WritableValue.h"
#include "WritableStackSlot.h"

namespace Lua {

	template<typename T>
	class WritableNumber : public WritableValue {
	public:

		typedef std::numeric_limits<Library::Number> EngineLimits;

		WritableNumber(T num) :
			_num(num)
		{}

		virtual void insertTo(State& state) const
		{
			WritableStackSlot slot(state);

			slot.prepare();
			slot.insertNumber((Library::Number) _num);
			slot.finish();
		}

	private:

		T _num;

	};

}