#pragma once

#include "StackSlot.h"

namespace Lua {

	/*
		This is a helper class for reading items from the stack.
		To read one item from the stack, exactly one local instance
		of this class is created.

		The item can be either a table field value (table level is greater than zero)
		or a value outside the table (table level is zero).

		After reading, the item is removed from the stack unless keep()
		function has been called.
	*/

	class ReadableStackSlot : public StackSlot {
	public:

		ReadableStackSlot(State& state) :
			StackSlot(state),
			_keep(false)
		{}

		~ReadableStackSlot()
		{
			// when the local variable holding this instance goes out of scope,
			// the class's destructor ensures that the stack top is set accordingly

			if (state().getTableLevel() == 0) {
				// gets a index of out-of-table value
				int valueIndex = getSlotTop();

				// the out-of-table item is removed by setting the top of the stack
				// to the item index minus one

				if (!_keep && valueIndex > 0) {
					valueIndex--;
				}

				state().setStackTop(valueIndex);
			}
			else {
				// gets a table index
				int tableIndex = getSlotTop();

				// the table item is removed by setting the top of the stack
				// back to the table index
				state().setStackTop(tableIndex);
			}

		}

		// prepares to read a item from the slot
		void prepare()
		{
			state().prepareReading();
		}

		// prepares to read a item with a specific type from the slot
		void prepare(int expectedType)
		{
			state().prepareReading(expectedType);
		}

		// finishes item reading
		void finish()
		{
			state().finishReading();
		}

		// causes item to remain on the stack after being read
		void keep()
		{
			state().noRemoval();
			_keep = true;
		}

		// gets a boolean value from the stack
		int getBoolean()
		{
			return lua_toboolean(
				state().getL(),
				state().getStackTop()
			);
		}

		// gets a numeric value from the stack
		lua_Number getNumber()
		{
			return lua_tonumber(
				state().getL(),
				state().getStackTop()
			);
		}

		// gets a integer value from the stack
		lua_Integer getInteger()
		{
			return lua_tointeger(
				state().getL(),
				state().getStackTop()
			);
		}

		// gets a table from the stack
		StackSlot& getTable()
		{
			return *this;
		}

		template<typename String>
		String getString()
		{
			return ToString<String>(state(), state().getStackTop());
		}

	private:

		// flag to keep item on the stack after being read
		bool _keep;

	};

}