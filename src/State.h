#pragma once

#include <stdexcept>
#include <algorithm>
#include <utility>
#include <string>
#include <functional>
#include <memory>
#include <deque>
#include <sstream>
#include <limits>
#include <type_traits>

extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}

#include "TypeToName.h"

namespace Lua {

	class State {
	public:

		typedef std::deque<std::string> Keys;

		static const int StackTop = -1;

		State(lua_State* L = nullptr) :
			_L(L),
			_autoClose(false),
			_keysPtr(nullptr),
			_tableLevel(0),
			_closureLevel(0),
			_noRemoval(false)
		{}

		~State()
		{
			if (_autoClose) {
				lua_close(_L);
			}
		}

		void swap(State& state)
		{
			std::swap(_L, state._L);
			std::swap(_autoClose, state._autoClose);
		}

		lua_State* getL()
		{
			return _L;
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

		void noRemoval()
		{
			_noRemoval = true;
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

		void prepareReading(int expectedType)
		{
			if (getTableLevel() > 0) {
				getValueFromField();
			}

			int top = getStackTop();

			if (top == 0) {
				throw std::logic_error(
					"stack is empty, expected " + TypeToName(expectedType)
				);
			}

			int type = lua_type(getL(), top);

			if (type != expectedType) {
				pop();
				throw std::logic_error(
					TypeToName(expectedType) + " expected, got " + TypeToName(type) + " type"
				);
			}

		}

		void finishReading()
		{
			if (_noRemoval) {
				_noRemoval = false;
			}
			else {
				pop();
			}
		}

		bool isReadyForWritingParam()
		{
			return getTableLevel() == 0 && getClosureLevel() == 0;
		}

		void prepareWriting()
		{
			// ensures that there are at least LUA_MINSTACK free stack slots in the stack.
			if (!lua_checkstack(getL(), LUA_MINSTACK)) {
				throw std::runtime_error("not enought memory");
			}
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
				bool hasMetatable = lua_getmetatable(getL(), StackTop) != 0;

				if (!hasMetatable) {
					throw std::logic_error("metatable is not set");
				}

				// get a value from the metatable field with the given key name

				lua_getfield(
					getL(),
					StackTop,
					currKey.c_str()
				);

				// remove the metatable
				removeElementAt(StackTop - 1);

			}
			else {

				// get a value from the field with the given key name

				lua_getfield(
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

			if (!lua_istable(getL(), StackTop - 1)) {
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
				bool hasMetatable = lua_getmetatable(getL(), StackTop - 1) != 0;

				if (!hasMetatable) {
					// insert a new table at top of the stack
					lua_createtable(getL(), 0, 0);
				}

				// place the table before the value
				moveTopElementTo(StackTop - 1);

				// set the value as a metatable field with the given key name
				lua_setfield(
					getL(),
					StackTop - 1,
					currKey.c_str()
				);

				if (!hasMetatable) {
					// set the new table as a value metatable
					lua_setmetatable(getL(), State::StackTop - 1);
				}
				else {
					// pop the value metatable from the stack
					pop();
				}

			}
			else {

				// set the value as a table field with the given key name
				lua_setfield(
					getL(),
					StackTop - 1,
					currKey.c_str()
				);

			}

			// remove the used key name from the list
			keys().pop_front();
		}

		bool isOpen()
		{
			return _L != nullptr;
		}

		void open()
		{
			if (!isOpen()) {
				_L = luaL_newstate();
				_autoClose = (_L != nullptr);
			}
		}

		void loadChunk(
			lua_Reader readerCb,
			void *reader,
			const std::string& chunkName = "",
			const std::string& mode = "bt"
		)
		{
			int status = lua_load(
				getL(),
				readerCb,
				reader,
				chunkName.c_str(),
				mode.c_str()
			);

			reportStatus(status);
		}

		void dumpChunk(
			lua_Writer writerCb,
			void* writer,
			bool strip = false
		)
		{
			int status = lua_dump(
				getL(),
				writerCb,
				writer,
				strip
			);

			reportStatus(status);
		}

		void pcall(
			int nargs = 0,
			int nresults = LUA_MULTRET,
			int errfunc = 0
		)
		{
			int status = lua_pcall(
				getL(),
				nargs,
				nresults,
				errfunc
			);

			reportStatus(status);
		}

		void openStdLibs()
		{
			luaL_openlibs(getL());
		}

		int getStackTop()
		{
			return lua_gettop(getL());
		}

		void setStackTop(int index)
		{
			lua_settop(getL(), index);
		}

		void pushGlobal(const char* name)
		{
			lua_getglobal(getL(), name);
		}

		void moveTopElementTo(int index, int repeat = 1)
		{
			for (int i = 0; i < repeat; i++) {
				lua_insert(getL(), index);
			}
		}

		void removeElementAt(int index, int num = 1)
		{
			for (int i = 0; i < num; i++) {
				lua_remove(getL(), index + i);
			}
		}

		void setGlobal(const char* name)
		{
			lua_setglobal(getL(), name);
		}

		int getType(int index)
		{
			return lua_type(getL(), index);
		}

		void raiseError(const std::string& errorMessage)
		{
			lua_pushlstring(
				getL(),
				errorMessage.c_str(),
				errorMessage.length()
			);

			lua_error(getL());
		}

		void pushElementFrom(int index)
		{
			lua_pushvalue(getL(), index);
		}

		void pushGlobal(const std::string& name)
		{
			lua_getglobal(getL(), name.c_str());
		}

		void setGlobal(const std::string& name)
		{
			lua_setglobal(getL(), name.c_str());
		}

		void pop(int num = 1)
		{
			lua_pop(getL(), num);
		}

	private:

		lua_State* _L;
		bool _autoClose;
		Keys* _keysPtr;
		int _tableLevel;
		int _closureLevel;
		bool _noRemoval;

		void reportStatus(int status)
		{
			switch (status) {
				case LUA_OK:
					// success
				break;
				case LUA_ERRRUN:
				case LUA_ERRSYNTAX: {
					// the error message is at the stack top
					std::string err(lua_tostring(getL(), StackTop));
					// pop the error message
					pop();
					throw std::runtime_error(err);
				}
				break;
				case LUA_ERRMEM:
					throw std::runtime_error("memory allocation error");
				break;
				case LUA_ERRERR:
					throw std::runtime_error("error while running the message handler");
				break;
				default:
					throw std::runtime_error("unknown error");
			}
		}

	};

}
