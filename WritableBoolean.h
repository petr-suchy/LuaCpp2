#pragma once

#include "WritableValue.h"

namespace Lua {

	class WritableBoolean : public WritableValue {
	public:

		WritableBoolean(bool val) :
			_val(val)
		{}

		virtual void insertTo(State& state)
		{
			state.prepareWriting();
			lua_pushboolean(state.getL(), _val);
			state.finishWriting();
		}

	private:

		bool _val;

	};

}