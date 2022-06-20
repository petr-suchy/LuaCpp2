#pragma once

#include "State.h"

namespace Lua {

	// the abstract function implementation
	class FunctionImpl {
	public:

		// shared pointer to the implementation
		typedef std::shared_ptr<FunctionImpl> SharedPtr;

		// inserts the function to the stack
		virtual void insertTo(State&) = 0;

	};

}