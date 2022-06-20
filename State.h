#pragma once

#include <stdexcept>
#include <algorithm>
#include <utility>
#include <string>
#include <functional>
#include <memory>
#include <deque>

extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}

namespace Lua {

	static std::string TypeToName(int type)
	{
		std::string name;

		switch (type) {
			case LUA_TNIL: name = "nil"; break;
			case LUA_TBOOLEAN: name = "boolean"; break;
			case LUA_TLIGHTUSERDATA: name = "lightuserdata"; break;
			case LUA_TNUMBER: name = "number"; break;
			case LUA_TSTRING: name = "string"; break;
			case LUA_TTABLE: name = "table"; break;
			case LUA_TFUNCTION: name = "function"; break;
			case LUA_TUSERDATA: name = "userdata"; break;
			case LUA_TTHREAD: name = "thread"; break;
			default: name = "unknown"; break;
		}

		return name;
	}

	class State {
	public:

		typedef std::deque<std::string> Keys;

		static const int StackTop = -1;

		State(lua_State* L = nullptr) :
			_L(L),
			_autoClose(false),
			_tableLevel(0),
			_noRemoval(false),
			_numOfUpValues(0)
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
			return _keys;
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
			return (int) _closureLevels.size();
		}

		int getNumOfUpValues()
		{
			return _numOfUpValues;
		}

		void enterClosure()
		{
			_closureLevels.push_back(
				ClosureLevel{
					getStackTop(),
					_tableLevel
				}
			);

			_tableLevel = 0;
		}

		void leaveClosure()
		{
			ClosureLevel currLevel = _closureLevels.back();
			_closureLevels.pop_back();

			_numOfUpValues = getStackTop() - currLevel.stackTop;
			_tableLevel = currLevel.tableLevel;
		}

		void noRemoval()
		{
			_noRemoval = true;
		}

		bool isReadyForReadingParam()
		{
			return getTableLevel() == 0 && getClosureLevel() == 0;
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
			// TODO: checkstack
		}

		// this function must always be called after inserting a value into the stack
		void finishWriting()
		{
			bool isTableAtTop = lua_istable(getL(), StackTop);

			// if a value at the top of the stack is a table field
			// and it int't a nested table, set this value as a table field

			if (getTableLevel() > 0 && !isTableAtTop) {
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

			// get the table field name from the key list 

			if (keys().empty()) {
				throw std::logic_error("a key expected for the table field");
			}

			std::string currKey = keys().front();

			// get a value from the field with the given key name

			lua_getfield(
				getL(),
				top,
				currKey.c_str()
			);

			// remove the used key name from the list
			keys().pop_front();
		}

		// sets the value at the of the stack as a table field
		void setValueAsField()
		{
			// the table for the value is at the top of the stack
			// just before the value

			int top = getStackTop();

			if (top < 2 || !lua_istable(getL(), top - 1)) {
				throw std::logic_error("a table expected for the table field");
			}

			int tableIndex = top - 1;

			// get the table field name from the key list 

			if (keys().empty()) {
				throw std::logic_error("a key expected for the table field");
			}

			std::string currKey = keys().front();

			// set the values as a table field with the given key name

			lua_setfield(
				getL(),
				tableIndex,
				currKey.c_str()
			);

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
			const char* chunkName,
			const char* mode = nullptr
		)
		{
			int status = lua_load(
				getL(),
				readerCb,
				reader,
				chunkName,
				mode
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

		struct ClosureLevel {
			int stackTop;
			int tableLevel;
		};

		lua_State* _L;
		bool _autoClose;
		Keys _keys;
		int _tableLevel;
		bool _noRemoval;
		std::deque<ClosureLevel> _closureLevels;
		int _numOfUpValues;

		void reportStatus(int status)
		{
			switch (status) {
				case LUA_OK:
					// success
				break;
				case LUA_ERRRUN:
					// the error message is at the stack top
					throw std::runtime_error(lua_tostring(getL(), -1));
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
