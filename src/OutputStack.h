#pragma once

#include "State.h"

namespace Lua {

	class OutputStack {
	public:

		OutputStack(State& state) :
			_state(state)
		{}

		void createTable()
		{
			lua_createtable(_state.getL(), 0, 0);
		}

		void pushNil()
		{
			lua_pushnil(_state.getL());
		}

		void pushBoolean(int boolVal)
		{
			lua_pushboolean(_state.getL(), boolVal);
		}

		void pushNumber(lua_Number num)
		{
			lua_pushnumber(_state.getL(), num);
		}

		void pushString(const char* str, size_t len)
		{
			lua_pushlstring(_state.getL(), str, len);
		}

		void setTable()
		{
			lua_settable(_state.getL(), -3);
		}

	private:

		State& _state;

	};

}