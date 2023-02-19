#pragma once

#include "ReadableValue.h"
#include "WritableValue.h"
#include "ReadableStackSlot.h"
#include "WritableStackSlot.h"

namespace Lua {

	class Reference : public ReadableValue, public WritableValue {
	public:

		Reference() :
			_ref(
				std::make_shared<Ref>(
					State::SharedPtr{},
					LUA_REFNIL
				)
			)
		{}

		virtual void getFrom(State& state)
		{
			ReadableStackSlot slot(state);

			slot.prepare();

			int ref = slot.getReference();

			if (ref == LUA_REFNIL) {
				throw std::runtime_error("nil reference returned");
			}

			_ref = std::make_shared<Ref>(
				state.getSharedPtr(),
				ref
			);

			slot.finish();
		}

		virtual void insertTo(State& state) const
		{
			auto refStatePtr = _ref->getWeakStatePtr().lock();

			if (!refStatePtr) {
				throw std::logic_error(
					"reference state is destroyed"
				);
			}

			State refState(refStatePtr);

			if (refState.getL() != state.getL()) {
				throw std::logic_error(
					"reference does not belong to this state"
				);
			}

			WritableStackSlot slot(state);

			slot.prepare();
			slot.insertReference(_ref->get());
			slot.finish();
		}

	private:

		class Ref {
		public:

			Ref(State::WeakPtr weakStatePtr, int ref) :
				_weakStatePtr(weakStatePtr),
				_ref(ref)
			{}

			~Ref()
			{
				if (auto statePtr = _weakStatePtr.lock()) {
					luaL_unref(
						statePtr->getL(),
						LUA_REGISTRYINDEX,
						_ref
					);
				}
			}

			State::WeakPtr getWeakStatePtr()
			{
				return _weakStatePtr;
			}

			int get()
			{
				return _ref;
			}

		private:

			State::WeakPtr _weakStatePtr;
			int _ref;

		};

		std::shared_ptr<Ref> _ref;

	};

}