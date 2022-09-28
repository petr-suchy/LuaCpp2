#pragma once

#include "ReadableValue.h"
#include "ReadableStackSlot.h"

namespace Lua {

	class ReadableBoolean : public ReadableValue {
	public:

		ReadableBoolean(bool& val) :
			_val(val)
		{}

		virtual void getFrom(State& state)
		{
			ReadableStackSlot slot(state);

			slot.prepare(LUA_TBOOLEAN);
			_val = (slot.getBoolean() != 0);
			slot.finish();
		}

	private:

		bool& _val;

	};

}