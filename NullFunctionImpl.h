#pragma once

#include "FunctionImpl.h"

namespace Lua {

	class NullFunctionImpl : public FunctionImpl {
	public:

		virtual void insertTo(State&)
		{
			throw std::logic_error("null function");
		}

	};

}