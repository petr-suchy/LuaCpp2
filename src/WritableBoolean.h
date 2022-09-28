#pragma once

#include "WritableValue.h"
#include "WritableStackSlot.h"

namespace Lua {

	class WritableBoolean : public WritableValue {
	public:

		WritableBoolean(bool val) :
			_val(val)
		{}

		virtual void insertTo(State& state)
		{
			WritableStackSlot slot(state);
			
			slot.prepare();
			slot.insertBoolean(_val ? 1 : 0);
			slot.finish();
		}

	private:

		bool _val;

	};

}