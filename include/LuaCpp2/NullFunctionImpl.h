#pragma once

#include "FunctionImpl.h"

namespace Lua {

	class NullFunctionImpl : public FunctionImpl {
	public:

		virtual Library::State* getL()
		{
			return nullptr;
		}

		virtual void insertTo(State&) const
		{
			throw std::logic_error(
				"a null function cannot be inserted"
			);
		}

	};

}