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
			lua_pushvalue(
				state.getL(),
				lua_upvalueindex(_index)
			);
		}

	private:
		int _index;
	};

}