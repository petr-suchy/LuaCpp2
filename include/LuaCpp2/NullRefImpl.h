#pragma once

#include "RefImpl.h"

namespace Lua {

	// Null reference implementation.
	class NullRefImpl : public RefImpl {
	public:

		virtual Library::State* getL() const
		{
			return nullptr;
		}

		virtual void insertTo(State&) const
		{
			throw std::logic_error(
				"a null reference cannot be inserted to the stack"
			);
		}

	};

}