#pragma once

#include "State.h"

namespace Lua {

	class StackModifier {
	public:
		virtual void modify(State&) const = 0;
	};

}

static Lua::State& operator>> (
	Lua::State& state,
	const Lua::StackModifier& modifier
)
{
	modifier.modify(state);
	return state;
}

static Lua::State& operator<< (
	Lua::State& state,
	const Lua::StackModifier& modifier
)
{
	modifier.modify(state);
	return state;
}
