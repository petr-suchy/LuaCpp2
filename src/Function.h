#pragma once

#include "AbstractFunction.h"
#include "NullFunctionImpl.h"
#include "HostFunctionImpl.h"
#include "EngineFunctionImpl.h"
#include "ReadableStackSlot.h"

namespace Lua {

	// represents a function. It has a responsibility to insert
	// the function at the top of the stack. It delegates that
	// responsiblity to an internal implementation.

	class Function : public AbstractFunction {
	public:

		typedef HostFunctionImpl::FunctionPrototype Prototype;

		Function() :
			_impl(std::make_shared<NullFunctionImpl>())
		{}

		Function(const Prototype& func) :
			_impl(std::make_shared<HostFunctionImpl>(func))
		{}

		// gets the function from the stack
		virtual void getFrom(State& state)
		{
			ReadableStackSlot slot(state);

			slot.prepare(LUA_TFUNCTION);

			int ref = slot.getReference();

			if (ref == LUA_REFNIL) {
				throw std::runtime_error("nil reference returned");
			}

			_impl = std::make_shared<EngineFunctionImpl>(state, ref);

			slot.finish();
		}

		// inserts the function to the stack
		virtual void insertTo(State& state)
		{
			_impl->insertTo(state);
		}

	private:

		// shared pointer to the internal implementaion
		FunctionImpl::SharedPtr _impl;

	};

}