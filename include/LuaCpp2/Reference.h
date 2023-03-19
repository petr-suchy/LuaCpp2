#pragma once

#include "NullRefImpl.h"
#include "SetRefImpl.h"
#include "ReadableValue.h"
#include "WritableValue.h"
#include "ReadableStackSlot.h"
#include "WritableStackSlot.h"

namespace Lua {

	// Encapsulates a reference to an engine value.
	class Reference : public ReadableValue, public WritableValue {
	public:

		Reference() :
			_impl(std::make_shared<NullRefImpl>())
		{}

		virtual void getFrom(State& state)
		{
			ReadableStackSlot slot(state);

			slot.prepare();

			// get a reference to the value on the stack
			int ref = slot.getReference();

			if (Library::inst().isrefnil(ref)) {
				throw std::runtime_error("nil reference returned");
			}

			// replace the internal implementation with the implementation of
			// the set reference.
			_impl = std::make_shared<SetRefImpl>(state, ref);

			slot.finish();
		}

		virtual void insertTo(State& state) const
		{
			_impl->insertTo(state);
		}

	private:

		// Shared pointer to the internal implementaion.
		RefImpl::SharedPtr _impl;

	};

}