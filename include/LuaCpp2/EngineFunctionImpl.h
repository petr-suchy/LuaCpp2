#pragma once

#include "FunctionImpl.h"
#include "WritableStackSlot.h"

namespace Lua {

	// implementation for an engine (Lua) function
	class EngineFunctionImpl : public FunctionImpl {
	public:

		typedef int Reference;

		EngineFunctionImpl(State& state, Reference ref) :
			_weakStatePtr(state.getSharedPtr()),
			_ref(ref)
		{}

		~EngineFunctionImpl()
		{
			if (auto statePtr = _weakStatePtr.lock()) {
				// release both the function and the reference
				Library::inst().unref(statePtr->getL(), _ref);
			}
		}

		virtual State::WeakPtr getWeakStatePtr()
		{
			return _weakStatePtr;
		}
		
		virtual void insertTo(State& state) const
		{
			auto funcStatePtr = _weakStatePtr.lock();

			if (!funcStatePtr) {
				throw std::logic_error(
					"function state is destroyed"
				);
			}

			State funcState(funcStatePtr);

			if (funcState.getL() != state.getL()) {
				throw std::logic_error(
					"function does not belong to this state"
				);
			}

			WritableStackSlot slot(state);

			slot.prepare();
			slot.insertReference(_ref);
			slot.finish();
		}

	private:

		State::WeakPtr _weakStatePtr;
		Reference _ref;

	};

}