#pragma once

#include "State.h"
#include "TableIterator.h"

namespace Lua {

	class StackItem {
	public:

		StackItem(State& state, int index = 0) :
			_state(state),
			_index(index)
		{}

		int getIndex() const
		{
			return _index;
		}

		void setIndex(int index)
		{
			_index = index;
		}

		bool isBoolean() const
		{
			return _state.getType(_index) == LUA_TBOOLEAN;
		}

		bool isInteger() const
		{
			return false;
		}

		bool isNumber() const
		{
			return _state.getType(_index) == LUA_TNUMBER;
		}

		bool isString() const
		{
			return _state.getType(_index) == LUA_TSTRING;
		}

		bool isTable() const
		{
			return _state.getType(_index) == LUA_TTABLE;
		}

		lua_Number getNumber() const
		{
			// create a temporary copy of the value at the top of the stack
			lua_pushvalue(_state.getL(), _index);
			// convert the temporary value to a number
			lua_Number num = lua_tonumber(_state.getL(), _index);
			// remove the temporary value from the stack
			_state.pop();

			return num;
		}
		
		TableIterator<StackItem> getTable() const
		{
			return TableIterator<StackItem>(_state, _index);
		}
		
	private:

		State& _state;
		int _index;

	};

}