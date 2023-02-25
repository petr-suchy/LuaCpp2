#pragma once

#include "FunctionImpl.h"

namespace Lua {

	class NullFunctionImpl : public FunctionImpl {
	public:

		virtual State::WeakPtr getWeakStatePtr()
		{
			throw std::logic_error(
				"a null function has no weak pointer to the engine state"
			);

			return State::SharedPtr();
		}

		virtual void insertTo(State&) const
		{
			throw std::logic_error(
				"a null function cannot be inserted"
			);
		}

	};

}