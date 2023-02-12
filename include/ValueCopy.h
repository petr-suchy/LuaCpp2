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

		virtual void insertTo(State& state) const
		{
			WritableStackSlot slot(state);

			slot.prepare();

			{
				OutputStack ostack(state);

				_copy.transfer(ostack);
				ostack.finish();
			}

			slot.finish();
		}

	private:

		Lua::OutputCopy<Lua::OutputStack> _copy;

	};

}
