#pragma once

#include "FunctionImpl.h"

namespace Lua {

	class NullFunctionImpl : public FunctionImpl {
	public:

		virtual lua_State* getL()
		{
			throw std::logic_error(
				"null function has no raw pointer to engine state"
			);

			return NULL;
		}

		virtual void insertTo(State&) const
		{
			throw std::logic_error("null function");
		}

	};

}