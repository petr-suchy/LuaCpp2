#pragma once

#include "RefImpl.h"
#include "WritableStackSlot.h"

namespace Lua {

	// Implementation of the set reference.
	class SetRefImpl : public RefImpl {
	public:

		typedef int Ref;

		SetRefImpl(State& state, Ref ref) :
			_weakStatePtr(state.getSharedPtr()),
			_ref(ref)
		{}

		~SetRefImpl()
		{
			if (auto statePtr = _weakStatePtr.lock()) {
				Library::inst().unref(statePtr->getL(), _ref);
			}
		}

		virtual State::WeakPtr getWeakStatePtr()
		{
			return _weakStatePtr;
		}

		virtual void insertTo(State& state) const
		{
			auto refStatePtr = _weakStatePtr.lock();

			if (!refStatePtr) {
				throw std::logic_error(
					"reference state is destroyed"
				);
			}

			State refState(refStatePtr);

			if (refState.getL() != state.getL()) {
				throw std::logic_error(
					"refererence does not belong to this state"
				);
			}

			WritableStackSlot slot(state);

			slot.prepare();
			slot.insertReference(_ref);
			slot.finish();
		}

	private:

		State::WeakPtr _weakStatePtr;
		Ref _ref;

	};

}
