#pragma once

#include "StackSlot.h"
#include "ToString.h"

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

		// prepares to read an item with a specific type from the slot
		void prepare(Library::Type expectedType)
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
			_keep = true;
		}

		// gets a boolean value from the stack
		bool getBoolean()
		{
			return state().toBooleanAt(State::StackTop);
		}

		// gets a numeric value from the stack
		Library::Number getNumber()
		{
			return Library::inst().tonumber(
				state().getL(),
				state().getStackTop()
			);
		}

		// gets a integer value from the stack
		Library::Integer getInteger()
		{
			return state().toIntegerAt(State::StackTop);
		}

		// gets a string from the stack
		template<typename String>
		String getString()
		{
			return ToString<String>(state(), state().getStackTop());
		}

		// gets a userdata type from the stack
		template<typename Type>
		Type* getUserData()
		{
			void* rawUserDataPtr2Ptr = Library::inst().touserdata(
				state().getL(),
				state().getStackTop()
			);

			return *reinterpret_cast<Type**>(rawUserDataPtr2Ptr);
		}

		// gets a reference to the item on the stack
		int getReference()
		{
			// pops a value from the stack, stores it into
			// the registry with a fresh integer key, and returns
			// that key as "reference"
			return Library::inst().ref(state().getL());
		}

		// gets a table from the stack
		StackSlot& getTable()
		{
			return *this;
		}

	private:

		// flag to keep item on the stack after being read
		bool _keep;

	};

}