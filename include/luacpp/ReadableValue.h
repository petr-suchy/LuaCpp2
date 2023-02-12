#pragma once

#include "State.h"

namespace Lua {

	class ReadableValue {
	public:
		virtual void getFrom(State&) = 0;
	};

}

static Lua::State& operator>> (Lua::State& state, Lua::ReadableValue& val)
{
	val.getFrom(state);
	return state;
}