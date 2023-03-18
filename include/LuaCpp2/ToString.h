#pragma once

#include "State.h"

namespace Lua {

	template<typename String>
	String ToString(State& state, int index)
	{
		int origStackTop = state.getStackTop() - 1;
		int type = Library::inst().type(state.getL(), index);

		if (state.isValueAt(index, Library::Type::String) ||
			state.isValueAt(index, Library::Type::Integer) ||
			state.isValueAt(index, Library::Type::Number)
		) {

			size_t len = 0;

			const char* cstr = Library::inst().tolstring(
				state.getL(),
				state.getStackTop(),
				&len
			);

			return String(cstr, len);
		}

		if (
			!state.isValueAt(index, Library::Type::Table) &&
			type != LUA_TUSERDATA
		) {

			state.setStackTop(origStackTop);

			throw std::logic_error(
				"string expected, got " + TypeToName(type) + " type"
			);
		}

		// insert a metatable at the top of the stack
		bool hasMetatable = Library::inst().getmetatable(state.getL(), State::StackTop) != 0;

		if (!hasMetatable) {
			state.setStackTop(origStackTop);
			throw std::logic_error(
				TypeToName(type) + " cannot be converted to string"
			);
		}

		// insert __tostring metamethod at the top of the stack
		Library::inst().getfield(
			state.getL(),
			State::StackTop,
			"__tostring"
		);

		bool hasToString = state.isValueAt(State::StackTop, Library::Type::Function);

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

		if (
			!state.isValueAt(State::StackTop, Library::Type::String) &&
			!state.isValueAt(State::StackTop, Library::Type::Integer) &&
			!state.isValueAt(State::StackTop, Library::Type::Number)
		) {

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