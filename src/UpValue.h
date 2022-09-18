#pragma once

#include "ReadableValue.h"

namespace Lua {

	class UpValue : public ReadableValue {
	public:

		UpValue(int index) :
			_index(index)
		{}

		virtual void getFrom(State& state)
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