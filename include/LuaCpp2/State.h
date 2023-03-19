#pragma once

#include "StatePointer.h"

#include <algorithm>
#include <utility>
#include <string>
#include <functional>
#include <deque>
#include <sstream>
#include <limits>
#include <type_traits>
#include <memory>

namespace Lua {

	class State {
	public:

		typedef std::shared_ptr<AbstractStatePointer> SharedPtr;
		typedef std::weak_ptr<AbstractStatePointer> WeakPtr;

		typedef std::deque<std::string> Keys;

		static const int StackTop = -1;
		static const int MinStack = 20;

		State(SharedPtr ptr) :
			_ptr(ptr),
			_keysPtr(nullptr),
			_tableLevel(0),
			_closureLevel(0)
		{}

		State() :
			State(std::make_shared<NullStatePointer>())
		{}

		State(Library::State* L) :
			State(std::make_shared<AuxStatePointer>(L))
		{}

		// Returns true if the state is created, and false otherwise.
		bool isOpen()
		{
			return _ptr->isOpen();
		}

		// Gets a shared pointer to the state.
		SharedPtr getSharedPtr()
		{
			return _ptr;
		}

		// Gets a raw pointer to the state.
		Library::State* getL()
		{
			return _ptr->getL();
		}

		// Creates a new state if not already created.
		void open()
		{
			if (!isOpen()) {
				_ptr = std::make_shared<StatePointer>();
			}
		}

		// Loads all standard libraries into the state.
		void openStdLibs()
		{
			Library::inst().openlibs(getL());
		}

		/* Stack */

		// Returns the index of the top element in the stack.
		int getStackTop()
		{
			return Library::inst().gettop(getL());
		}

		// Sets the stack top to the given index.
		void setStackTop(int index)
		{
			Library::inst().settop(getL(), index);
		}

		// Ensures that there are extra free stack slots in the stack,
		// otherwise returns false.
		bool checkStack(int extra)
		{
			return Library::inst().checkstack(getL(), extra) != 0;
		}

		// Ensures that there are extra free stack slots in the stack,
		// otherwise throws an exception.
		void growStack(int extra)
		{
			if (!checkStack(1)) {
				throw std::runtime_error("not enought memory");
			}
		}

		/* Manipulation of elements */

		// Pops the given number of elements from the stack.
		void pop(int num = 1)
		{
			Library::inst().pop(getL(), num);
		}

		// Pushes a copy of the element at the given index onto the stack.
		void pushElementFrom(int index)
		{
			growStack(1);
			Library::inst().pushvalue(getL(), index);
		}

		// Moves the top element into the given index, shifting up the elements
		// above this index to open space.
		void moveTopElementTo(int index, int repeat = 1)
		{
			for (int i = 0; i < repeat; i++) {
				Library::inst().insert(getL(), index);
			}
		}

		// Removes the element at the given index, shifting down the elements
		// above this index to fill the gap.
		void removeElementAt(int index, int num = 1)
		{
			for (int i = 0; i < num; i++) {
				Library::inst().remove(getL(), index + i);
			}
		}

		/* Pushing elements */

		// Pushes a boolean value onto the stack.
		void pushBoolean(int b)
		{
			growStack(1);
			Library::inst().pushboolean(getL(), b);
		}

		// Pushes a closure onto the stack.
		void pushClosure(Library::CFunction fn, int n)
		{
			growStack(1);
			Library::inst().pushcclosure(getL(), fn, n);
		}

		// Pushes a function onto the stack.
		void pushFunction(Library::CFunction fn)
		{
			growStack(1);
			Library::inst().pushcfunction(getL(), fn);
		}

		// Pushes a integer value onto the stack.
		void pushInteger(Library::Integer n)
		{
			growStack(1);
			Library::inst().pushinteger(getL(), n);
		}

		// Pushes a nil value onto the stack.
		void pushNil()
		{
			growStack(1);
			Library::inst().pushnil(getL());
		}

		// Pushes a floating-point number onto the stack.
		void pushNumber(Library::Number n)
		{
			growStack(1);
			Library::inst().pushnumber(getL(), n);
		}

		// Pushes a string onto the stack.
		void pushString(const std::string& s)
		{
			growStack(1);
			Library::inst().pushlstring(getL(), s.c_str(), s.length());
		}

		/* Testing elements */

		// Returns the type of the value at the given acceptable index.
		int getTypeAt(int index)
		{
			return Library::inst().type(getL(), index);
		}

		// Returns the name of the given type.
		std::string typeToName(int type)
		{
			return std::string{Library::inst().typetoname(getL(), type)};
		}

		// Returns the name of the given type.
		std::string typeToName(Library::Type type)
		{
			return std::string{Library::inst().typetoname(type)};
		}

		// Returns true if the value at the given acceptable index has type boolean,
		// and false otherwise.
		bool isBooleanAt(int index)
		{
			return Library::inst().isboolean(getL(), index) != 0;
		}

		// Returns true if the value at the given acceptable index is a C function,
		// and false otherwise.
		bool isCFunctionAt(int index)
		{
			return Library::inst().iscfunction(getL(), index) != 0;
		}

		// Returns true if the value at the given acceptable index is a function
		// (either C or Lua), and false otherwise.
		bool isFunctionAt(int index)
		{
			return Library::inst().isfunction(getL(), index) != 0;
		}

		// Returns true if the value at the given acceptable index is a integer,
		// and false otherwise.
		bool isIntegerAt(int index)
		{
			return Library::inst().isinteger(getL(), index) != 0;
		}

		// Returns true if the value at the given acceptable index is a light userdata,
		// and false otherwise.
		bool isLightUserdataAt(int index)
		{
			return Library::inst().islightuserdata(getL(), index) != 0;
		}

		// Returns true if the value at the given acceptable index is nil,
		// and false otherwise.
		bool isNilAt(int index)
		{
			return Library::inst().isnil(getL(), index) != 0;
		}

		// Returns true if the value at the given acceptable index is a number
		// or a string convertible to a number, and false otherwise.
		bool isNumberAt(int index)
		{
			return Library::inst().isnumber(getL(), index) != 0;
		}

		// Returns true if the value at the given acceptable index is a string,
		// and false otherwise.
		bool isStringAt(int index)
		{
			return Library::inst().isstring(getL(), index) != 0;
		}

		// Returns true if the value at the given acceptable index is a table,
		// and false otherwise.
		bool isTableAt(int index)
		{
			return Library::inst().istable(getL(), index) != 0;
		}

		// Returns true if the value at the given acceptable index is a thread,
		// and false otherwise.
		bool isThreadAt(int index)
		{
			return Library::inst().isthread(getL(), index) != 0;
		}

		// Returns true if the value at the given acceptable index is a userdata
		// (either full or light), and false otherwise.
		bool isUserdataAt(int index)
		{
			return Library::inst().isuserdata(getL(), index) != 0;
		}

		// Returns true if the value at the given acceptable index is of the specified type,
		// and false otherwise.
		bool isValueAt(int index, Library::Type type)
		{
			return Library::inst().isvalue(getL(), index, type) != 0;
		}

		Keys& keys()
		{
			if (!_keysPtr) {
				throw std::logic_error("table keys are not set");
			}

			return *_keysPtr;
		}

		void swapKeys(Keys** keysPtr)
		{
			std::swap(_keysPtr, *keysPtr);
		}

		int getTableLevel()
		{
			return _tableLevel;
		}

		void enterTable()
		{
			_tableLevel++;
		}

		void leaveTable()
		{
			_tableLevel--;
		}

		int getClosureLevel()
		{
			return _closureLevel;
		}

		void enterClosure(int& tableLevel)
		{
			_closureLevel++;
			std::swap(_tableLevel, tableLevel);
		}

		void leaveClosure(int& tableLevel)
		{
			std::swap(_tableLevel, tableLevel);
			_closureLevel--;
		}

		bool isReadyForReadingParam()
		{
			return getTableLevel() == 0 && getClosureLevel() == 0;
		}

		void prepareReading()
		{
			if (getTableLevel() > 0) {
				getValueFromField();
			}

			int top = getStackTop();

			if (top == 0) {
				throw std::logic_error(
					"stack is empty"
				);
			}
		}

		void prepareReading(Library::Type expectedType)
		{
			if (getTableLevel() > 0) {
				getValueFromField();
			}

			if (getStackTop() == 0) {

				throw std::logic_error(
					std::string{"stack is empty, expected "} + Library::inst().typetoname(expectedType)
				);

			}

			if (!isValueAt(StackTop, expectedType)) {

				int actualType = Library::inst().type(getL(), StackTop);

				throw std::logic_error(
					std::string{Library::inst().typetoname(expectedType)}
						+ " expected, got "
						+ Library::inst().typetoname(getL(), actualType) + " type"
				);

			}

		}

		void finishReading()
		{
			// there is nothing to do here
		}

		bool isReadyForWritingParam()
		{
			return getTableLevel() == 0 && getClosureLevel() == 0;
		}

		void prepareWriting()
		{
			growStack(MinStack);
		}

		// this function must always be called after inserting a value into the stack
		void finishWriting()
		{
			// if a value at the top of the stack is a table field,
			// set this value as a table field

			if (getTableLevel() > 0) {
				setValueAsField();
			}

			// in other cases, the value is left at the top of the stack
		}

		// inserts a table field value at the top of the stack
		void getValueFromField()
		{
			// a table should be at the top of the stack

			int top = getStackTop();

			if (top == 0) {
				throw std::logic_error(
					"stack is empty, expected a table"
				);
			}

			// get a table field name from the key list 

			if (keys().empty()) {
				throw std::logic_error("a key expected for the table field");
			}

			std::string currKey = keys().front();
			bool isMetamethod = currKey[0] == '_';

			if (isMetamethod) {

				// insert a metatable at top of the stack
				bool hasMetatable = Library::inst().getmetatable(getL(), StackTop) != 0;

				if (!hasMetatable) {
					throw std::logic_error("metatable is not set");
				}

				// get a value from the metatable field with the given key name

				Library::inst().getfield(
					getL(),
					StackTop,
					currKey.c_str()
				);

				// remove the metatable
				removeElementAt(StackTop - 1);

			}
			else {

				// get a value from the field with the given key name

				Library::inst().getfield(
					getL(),
					StackTop,
					currKey.c_str()
				);

			}

			// remove the used key name from the list
			keys().pop_front();
		}

		// sets the value at the of the stack as a table field
		void setValueAsField()
		{

			if (getStackTop() < 2) {
				throw std::logic_error("too few arguments on the stack");
			}

			// the table for the value is at the top of the stack
			// just before the value

			if (!Library::inst().istable(getL(), StackTop - 1)) {
				throw std::logic_error("a table expected for the table field");
			}

			// get the table field name from the key list 

			if (keys().empty()) {
				throw std::logic_error("a key expected for the table field");
			}

			std::string currKey = keys().front();
			bool isMetamethod = currKey[0] == '_';
			
			if (isMetamethod) {

				// insert a value metatable at top of the stack
				bool hasMetatable = Library::inst().getmetatable(getL(), StackTop - 1) != 0;

				if (!hasMetatable) {
					// insert a new table at top of the stack
					Library::inst().createtable(getL(), 0, 0);
				}

				// place the table before the value
				moveTopElementTo(StackTop - 1);

				// set the value as a metatable field with the given key name
				Library::inst().setfield(
					getL(),
					StackTop - 1,
					currKey.c_str()
				);

				if (!hasMetatable) {
					// set the new table as a value metatable
					Library::inst().setmetatable(getL(), State::StackTop - 1);
				}
				else {
					// pop the value metatable from the stack
					pop();
				}

			}
			else {

				// set the value as a table field with the given key name
				Library::inst().setfield(
					getL(),
					StackTop - 1,
					currKey.c_str()
				);

			}

			// remove the used key name from the list
			keys().pop_front();
		}

		void loadChunk(
			Library::Reader readerCb,
			void *reader,
			const std::string& chunkName = "",
			const std::string& mode = "bt"
		)
		{
			int status = Library::inst().load(
				getL(),
				readerCb,
				reader,
				chunkName.c_str(),
				mode.c_str()
			);

			reportStatus(status);
		}

		void dumpChunk(
			Library::Writer writerCb,
			void* writer,
			bool strip = false
		)
		{
			int status = Library::inst().dump(
				getL(),
				writerCb,
				writer,
				(strip ? 1 : 0)
			);

			reportStatus(status);
		}

		void pcall(int nargs = 0)
		{
			reportStatus(
				Library::inst().pcall(getL(), nargs)
			);
		}

		void pcall(int nargs, int nresults)
		{
			reportStatus(
				Library::inst().pcall(getL(), nargs, nresults)
			);
		}

		void pushGlobal(const char* name)
		{
			Library::inst().getglobal(getL(), name);
		}

		void setGlobal(const char* name)
		{
			Library::inst().setglobal(getL(), name);
		}

		void raiseError(const std::string& errorMessage)
		{
			Library::inst().pushlstring(
				getL(),
				errorMessage.c_str(),
				errorMessage.length()
			);

			Library::inst().error(getL());
		}

		void pushGlobal(const std::string& name)
		{
			Library::inst().getglobal(getL(), name.c_str());
		}

		void setGlobal(const std::string& name)
		{
			Library::inst().setglobal(getL(), name.c_str());
		}

	private:

		SharedPtr _ptr;
		Keys* _keysPtr;
		int _tableLevel;
		int _closureLevel;

		// throws an exception in case of an error
		void reportStatus(int status)
		{
			if (status) {

				char err[256];

				size_t len = Library::inst().toerrorstring(
					getL(), status,
					err, sizeof(err)
				);

				throw std::runtime_error(std::string{err, len});
			}
		}

	};

}
