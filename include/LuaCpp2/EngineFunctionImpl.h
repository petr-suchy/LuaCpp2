#pragma once

#include "FunctionImpl.h"
#include "WritableStackSlot.h"

namespace Lua {

	// implementation for an engine (Lua) function
	class EngineFunctionImpl : public FunctionImpl {
	public:

		typedef int Reference;

		EngineFunctionImpl(State& state, Reference ref) :
			_L(state.getL()),
			_ref(ref)
		{}

		~EngineFunctionImpl()
		{
			if (Library::State* L = Library::inst().lockstate(_L)) {
				Library::inst().unref(L, _ref);
				Library::inst().close(L);
			}
		}

		virtual Library::State* getL()
		{
			return _L;
		}
		
		virtual void insertTo(State& state) const
		{
			if (_L != state.getL()) {
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

		Library::State* _L;
		Reference _ref;

	};

}