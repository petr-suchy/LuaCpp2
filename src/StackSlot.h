#pragma once

#include "State.h"

namespace Lua {

	class StackSlot {
	public:

		StackSlot(State& state) :
			_state(state),
			_top(state.getStackTop())
		{}

		State& state() { return _state; }
		int getSlotTop() { return _top; }

	private:

		State& _state;
		int _top;

	};

}