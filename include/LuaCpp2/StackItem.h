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
			return _state.isBooleanAt(_index);
		}

		bool isInteger() const
		{
			return _state.isIntegerAt(_index);
		}

		bool isNumber() const
		{
			return _state.isNumberAt(_index);
		}

		bool isString() const
		{
			return _state.isStringAt(_index);
		}

		bool isTable() const
		{
			return _state.isTableAt( _index);
		}

		bool getBoolean() const
		{
			// create a temporary copy of the value at the top of the stack
			_state.pushValueFrom(_index);

			// get the temporary value as a boolean value
			ReadableStackSlot slot(_state);
			slot.prepare();
			bool boolVal = slot.getBoolean();
			slot.finish();

			return boolVal;
		}

		Library::Integer getInteger() const
		{
			// create a temporary copy of the value at the top of the stack
			_state.pushValueFrom(_index);

			// get the temporary value as an integer
			ReadableStackSlot slot(_state);
			slot.prepare();
			Library::Integer num = slot.getInteger();
			slot.finish();

			return num;
		}

		Library::Number getNumber() const
		{
			// create a temporary copy of the value at the top of the stack
			_state.pushValueFrom(_index);

			// get the temporary value as a number
			ReadableStackSlot slot(_state);
			slot.prepare();
			Library::Number num = slot.getNumber();
			slot.finish();

			return num;
		}

		std::string getString() const
		{
			// create a temporary copy of the value at the top of the stack
			_state.pushValueFrom(_index);

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