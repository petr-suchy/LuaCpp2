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
			// release both the function and the reference
			luaL_unref(_L, LUA_REGISTRYINDEX, _ref);
		}

		virtual void insertTo(State& state)
		{
			if (state.getL() != _L) {
				throw std::logic_error("invalid function reference");
			}

			WritableStackSlot slot(state);

			slot.prepare();
			// pushes the function associated with the reference
			// onto the stack
			lua_rawgeti(state.getL(), LUA_REGISTRYINDEX, _ref);
			slot.finish();
		}

	private:

		lua_State* _L;
		Reference _ref;

	};

}