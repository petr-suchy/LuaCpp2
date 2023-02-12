#pragma once

#include "StackItem.h"

namespace Lua {

	class InputStack {
	public:

		InputStack(State& state, int startIndex, int count) :
			_state(state),
			_startIndex(startIndex),
			_count(count),
			_currItem(state, startIndex)
		{}

		bool atEnd() const
		{
			int endIndex = _startIndex + _count;
			int stackTop = _state.getStackTop();

			if (endIndex > stackTop + 1) {
				endIndex = stackTop + 1;
			}

			return _currItem.getIndex() == endIndex;
		}

		const StackItem& curr() const
		{
			return _currItem;
		}

		void next()
		{
			_currItem.setIndex(_currItem.getIndex() + 1);
		}

	private:

		State& _state;
		int _startIndex;
		int _count;
		StackItem _currItem;

	};

}