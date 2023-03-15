#pragma once

#include "StackSlot.h"

namespace Lua {

	/*
		This is a helper class for writing items to the stack.
		To write one item to the stack, exactly one local instance
		of this class is created.

		The item can be either a table field value (table level is greater than zero)
		or a value outside the table (table level is zero).

		After writing, the item is left on the stack when finish()
		function has been called.
	*/

	class WritableStackSlot : public StackSlot {
	public:

		WritableStackSlot(State& state) :
			StackSlot(state),
			_finished(false)
		{}

		~WritableStackSlot()
		{
			// when the local variable holding this instance goes out of scope,
			// the class's destructor ensures that the stack top is set accordingly

			if (state().getTableLevel() == 0) {
				// gets the previous item index
				int prevIndex = getSlotTop();

				// an item with finished write is left on the stack by setting
				// the top of the stack to the previous index plus one
				if (_finished) {
					prevIndex++;
				}

				// an item with unfinished write is removed by setting
				// the top of the stack back to the previous index
				state().setStackTop(prevIndex);
			}
			else {
				// gets a table index
				int tableIndex = getSlotTop();

				// an item with unfinished write is removed by setting
				// the top of the stack back to the table index
				state().setStackTop(tableIndex);
			}

		}

		// prepares to write a item to the slot
		void prepare()
		{
			state().prepareWriting();
		}

		// finishes item writing
		void finish()
		{
			int expectedTop = getSlotTop() + 1;

			if (state().getStackTop() < expectedTop) {
				throw std::logic_error("missing item to store in the stack");
			}
			else if (state().getStackTop() > expectedTop) {
				throw std::logic_error("excess items to store in the stack");
			}

			state().finishWriting();

			_finished = true;
		}

		// insert the nil vlaue to the stack
		void insertNil()
		{
			Library::inst().pushnil(state().getL());
		}

		// inserts a boolean value to the stack
		void insertBoolean(bool boolVal)
		{
			Library::inst().pushboolean(state().getL(), boolVal);
		}

		// inserts a numeric value to the stack
		void insertNumber(Library::Number num)
		{
			Library::inst().pushnumber(state().getL(), num);
		}

		// inserts a integer value to the stack
		void insertInteger(Library::Integer num)
		{
			Library::inst().pushinteger(state().getL(), num);
		}

		// inserts a closure to the stack
		void insertClosure(Library::CFunction func, int numOfUpValues)
		{
			Library::inst().pushcclosure(
				state().getL(),
				func,
				numOfUpValues
			);
		}

		// inserts a string to the stack
		template<typename String>
		void insertString(const String& str)
		{
			Library::inst().pushlstring(state().getL(), str.c_str(), str.length());
		}

		// inserts a userdata type to the stack
		template<typename Type>
		void insertUserData(Type* ptr)
		{
			// allocate a userdata pointer to the wrapped type pointer
			// and insert it at the top of the stack as userdata
			Type** userDataPtr2Ptr =  reinterpret_cast<Type**>(
				lua_newuserdata(state().getL(), sizeof(Type*))
			);

			if (!userDataPtr2Ptr) {
				throw std::runtime_error("not enough memory");
			}

			// set the userdata pointer to the wrapped type pointer
			*userDataPtr2Ptr = ptr;
		}

		// inserts an item associated with the reference to the stack
		void insertReference(int ref)
		{
			lua_rawgeti(state().getL(), LUA_REGISTRYINDEX, ref);
		}

		// inserts a new table to the stack
		StackSlot& insertTable()
		{
			Library::inst().createtable(state().getL(), 0, 0);
			return *this;
		}

	private:

		// flag to keep item on the stack after being written
		bool _finished;

	};

}