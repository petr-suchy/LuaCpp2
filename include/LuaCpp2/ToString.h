#pragma once

#include "State.h"

namespace Lua {

	template<typename String>
	String ToString(State& state, int index)
	{
		int origStackTop = state.getStackTop() - 1;
		int type = state.getTypeAt(index);

		if (state.isValueAt(index, Library::Type::String) ||
			state.isValueAt(index, Library::Type::Integer) ||
			state.isValueAt(index, Library::Type::Number)
		) {
			return state.toStringAt<String>(State::StackTop);
		}

		if (
			!state.isValueAt(index, Library::Type::Table) &&
			!state.isValueAt(index, Library::Type::Userdata)
		) {

			state.setStackTop(origStackTop);

			throw std::logic_error(
				"string expected, got " + state.typeToName(type) + " type"
			);
		}

		// insert a metatable at the top of the stack

		if (!state.getMetatableAt(State::StackTop)) {

			state.setStackTop(origStackTop);

			throw std::logic_error(
				state.typeToName(type) + " cannot be converted to string"
			);
		}

		// insert __tostring metamethod at the top of the stack
		state.getFieldAt(State::StackTop, "__tostring");

		bool hasToString = state.isValueAt(State::StackTop, Library::Type::Function);

		if (!hasToString) {

			state.setStackTop(origStackTop);

			throw std::logic_error(
				state.typeToName(type) + " has not __tostring metamethod"
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
		state.removeValueAt(State::StackTop - 2, 2);

		return state.toStringAt<String>(State::StackTop);
	}

}