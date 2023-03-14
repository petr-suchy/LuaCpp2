#pragma once

#include "State.h"

namespace Lua {

	template<typename String>
	String ToString(State& state, int index)
	{
		int origStackTop = state.getStackTop() - 1;
		int type = Library::inst().type(state.getL(), index);

		if (type == LUA_TSTRING || type == LUA_TNUMBER) {

			size_t len = 0;

			const char* cstr = Library::inst().tolstring(
				state.getL(),
				state.getStackTop(),
				&len
			);

			return String(cstr, len);
		}

		if (type != LUA_TTABLE && type != LUA_TUSERDATA) {
			state.setStackTop(origStackTop);
			throw std::logic_error(
				"string expected, got " + TypeToName(type) + " type"
			);	
		}

		// insert a metatable at the top of the stack
		bool hasMetatable = lua_getmetatable(state.getL(), State::StackTop) != 0;

		if (!hasMetatable) {
			state.setStackTop(origStackTop);
			throw std::logic_error(
				TypeToName(type) + " cannot be converted to string"
			);
		}

		// insert __tostring metamethod at the top of the stack
		lua_getfield(
			state.getL(),
			State::StackTop,
			"__tostring"
		);

		bool hasToString = Library::inst().type(state.getL(), State::StackTop) == LUA_TFUNCTION;

		if (!hasToString) {
			state.setStackTop(origStackTop);
			throw std::logic_error(
				TypeToName(type) + " has not __tostring metamethod"
			);
		}

		// call __tostring metamethod

		try {
			state.pcall(0, 1);
		}
		catch (...) {
			state.setStackTop(origStackTop);
			throw;
		}

		int resultType = Library::inst().type(state.getL(), State::StackTop);

		if (resultType != LUA_TSTRING && resultType != LUA_TNUMBER) {
			state.setStackTop(origStackTop);
			throw std::logic_error(
				"__tostring metamethod returned no string value"
			);
		}

		// remove metatable and value
		state.removeElementAt(State::StackTop - 2, 2);

		size_t len = 0;

		const char* cstr = Library::inst().tolstring(
			state.getL(),
			state.getStackTop(),
			&len
		);
		
		return String(cstr, len);
	}

}