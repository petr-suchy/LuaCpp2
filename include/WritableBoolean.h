#pragma once

#include "WritableValue.h"
#include "WritableStackSlot.h"

namespace Lua {

	class WritableBoolean : public WritableValue {
	public:

		WritableBoolean(bool val) :
			_val(val)
		{}

		virtual void insertTo(State& state) const
		{
			WritableStackSlot slot(state);
			
			slot.prepare();
			slot.insertBoolean(_val);
			slot.finish();
		}

	private:

		bool _val;

	};

}