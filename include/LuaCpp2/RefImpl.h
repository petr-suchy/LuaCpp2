#pragma once

#include "State.h"

namespace Lua {

	// Abstract reference implementation.
	class RefImpl {
	public:

		// Shared pointer to the abstract implementation.
		typedef std::shared_ptr<RefImpl> SharedPtr;

		// Gets a pointer to the engine state.
		virtual Library::State* getL() const = 0;

		// Inserts the value associated with the reference to the stack.
		virtual void insertTo(State&) const = 0;

	};

}
