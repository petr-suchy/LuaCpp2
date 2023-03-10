#pragma once

#include "State.h"

namespace Lua {

	// the abstract function implementation
	class FunctionImpl {
	public:

		// shared pointer to the implementation
		typedef std::shared_ptr<FunctionImpl> SharedPtr;

		// gets a weak pointer to the engine state
		virtual State::WeakPtr getWeakStatePtr() = 0;

		// inserts the function to the stack
		virtual void insertTo(State&) const = 0;

	};

}