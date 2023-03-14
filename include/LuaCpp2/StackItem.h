#pragma once

#include "State.h"
#include "TableIterator.h"
#include "ReadableStackSlot.h"

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
			return Library::inst().isboolean(_state.getL(), _index) != 0;
		}

		bool isInteger() const
		{
			return lua_isinteger(_state.getL(), _index) != 0;
		}

		bool isNumber() const
		{
			return Library::inst().isnumber(_state.getL(), _index) != 0;
		}

		bool isString() const
		{
			return lua_isstring(_state.getL(), _index) != 0;
		}

		bool isTable() const
		{
			return lua_istable(_state.getL(), _index) != 0;
		}

		bool getBoolean() const
		{
			// create a temporary copy of the value at the top of the stack
			_state.pushElementFrom(_index);

			// get the temporary value as a boolean value
			ReadableStackSlot slot(_state);
			slot.prepare();
			bool boolVal = slot.getBoolean();
			slot.finish();

			return boolVal;
		}

		lua_Integer getInteger() const
		{
			// create a temporary copy of the value at the top of the stack
			_state.pushElementFrom(_index);

			// get the temporary value as an integer
			ReadableStackSlot slot(_state);
			slot.prepare();
			lua_Integer num = slot.getInteger();
			slot.finish();

			return num;
		}

		lua_Number getNumber() const
		{
			// create a temporary copy of the value at the top of the stack
			_state.pushElementFrom(_index);

			// get the temporary value as a number
			ReadableStackSlot slot(_state);
			slot.prepare();
			lua_Number num = slot.getNumber();
			slot.finish();

			return num;
		}

		std::string getString() const
		{
			// create a temporary copy of the value at the top of the stack
			_state.pushElementFrom(_index);

			// get the temporary value as a string
			ReadableStackSlot slot(_state);
			slot.prepare();
			std::string str = slot.getString<std::string>();
			slot.finish();

			return str;
		}

		TableIterator<StackItem> getTableIterator() const
		{
			return TableIterator<StackItem>(_state, _index);
		}
		
	private:

		State& _state;
		int _index;

	};

}