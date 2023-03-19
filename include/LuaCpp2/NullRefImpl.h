#pragma once

#include "RefImpl.h"

namespace Lua {

	// Null reference implementation.
	class NullRefImpl : public RefImpl {
	public:

		virtual State::WeakPtr getWeakStatePtr()
		{
			throw std::logic_error(
				"a null reference has no weak pointer to the engine state"
			);

			return State::SharedPtr();
		}

		virtual void insertTo(State&) const
		{
			throw std::logic_error(
				"a null reference cannot be inserted to the stack"
			);
		}

	};

}