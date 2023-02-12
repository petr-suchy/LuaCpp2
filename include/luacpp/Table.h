#pragma once

#include "StackSlot.h"

namespace Lua {

	class Table {
	public:

		Table(StackSlot& slot) :
			_state(slot.state()),
			_keysPtr(&_keys)
		{
			_state.enterTable();
			_state.swapKeys(&_keysPtr);
		}

		~Table()
		{
			_state.swapKeys(&_keysPtr);
			_state.leaveTable();
		}

		State& state()
		{
			return _state;
		}

	private:

		State& _state;
		State::Keys _keys;
		State::Keys* _keysPtr;

	};

}