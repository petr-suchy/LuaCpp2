#pragma once

#include "ReadableValue.h"
#include "WritableValue.h"
#include "ReadableStackSlot.h"
#include "WritableStackSlot.h"

#include "TransferStack.h"
#include "OutputCopy.h"

namespace Lua {

	class ValueCopy : public ReadableValue, public WritableValue {
	public:

		virtual void getFrom(State& state)
		{
			ReadableStackSlot slot(state);

			slot.prepare();

			_copy.clear();
			Transfer(state, _copy);

			slot.finish();
		}

		virtual void insertTo(State& state)
		{
			WritableStackSlot slot(state);

			slot.prepare();
			_copy.transfer(Lua::OutputStack{state});
			slot.finish();
		}

	private:

		Lua::OutputCopy<Lua::OutputStack> _copy;

	};

}