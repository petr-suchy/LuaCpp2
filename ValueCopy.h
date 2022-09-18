#pragma once

#include "ReadableValue.h"
#include "WritableValue.h"

#include "TransferStack.h"
#include "OutputCopy.h"

namespace Lua {

	class ValueCopy : public ReadableValue, public WritableValue {
	public:

		virtual void getFrom(State& state)
		{
			state.prepareReading();
			_copy.clear();
			Transfer(state, _copy);
			state.finishReading();
		}

		virtual void insertTo(State& state)
		{
			state.prepareWriting();
			_copy.transfer(Lua::OutputStack{state});
			state.finishWriting();
		}

	private:

		Lua::OutputCopy<Lua::OutputStack> _copy;

	};

}