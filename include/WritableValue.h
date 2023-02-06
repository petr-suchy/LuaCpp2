#pragma once

#include "State.h"

namespace Lua {

	class WritableValue {
	public:
		virtual void insertTo(State&) = 0;
	};

}

static Lua::State& operator<< (Lua::State& state, Lua::WritableValue& val)
{
	val.insertTo(state);
	return state;
}