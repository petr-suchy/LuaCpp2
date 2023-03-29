#pragma once

#include "RefImpl.h"
#include "WritableStackSlot.h"

namespace Lua {

	// Implementation of the set reference.
	class SetRefImpl : public RefImpl {
	public:

		typedef int Ref;

		SetRefImpl(State& state, Ref ref) :
			_L(state.getL()),
			_ref(ref)
		{}

		~SetRefImpl()
		{
			if (Library::State* L = Library::inst().lockstate(_L)) {
				Library::inst().unref(L, _ref);
				Library::inst().close(L);
			}
		}

		virtual Library::State* getL() const
		{
			return _L;
		}

		virtual void insertTo(State& state) const
		{
			if (_L != state.getL()) {
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

		Library::State* _L;
		Ref _ref;

	};

}
