#pragma once

#include "StackModifier.h"

namespace Lua {

	class UpValue : public StackModifier {
	public:

		UpValue(int index) :
			_index(index)
		{}

		virtual void modify(State& state) const
		{
			state.pushElementFrom(state.upvalueIndex(_index));
		}

	private:
		int _index;
	};

}