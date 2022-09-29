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
					nullptr,
					LUA_REFNIL
				)
			)
		{}

		virtual void getFrom(State& state)
		{
			ReadableStackSlot slot(state);

			slot.prepare();
			slot.state().noRemoval();

			// pops a value from the stack, stores it into
			// the registry with a fresh integer key, and returns
			// that key as "reference"
			int ref = luaL_ref(state.getL(), LUA_REGISTRYINDEX);

			_ref = std::make_shared<Ref>(state.getL(), ref);

			if (ref == LUA_REFNIL) {
				throw std::runtime_error("nil reference returned");
			}

			slot.finish();
		}

		virtual void insertTo(State& state)
		{
			if (state.getL() != _ref->getL()) {
				throw std::logic_error("invalid reference");
			}

			WritableStackSlot slot(state);

			slot.prepare();
			// pushes a value associated with the reference
			// onto the stack
			lua_rawgeti(state.getL(), LUA_REGISTRYINDEX, _ref->get());
			slot.finish();
		}

	private:

		class Ref {
		public:

			Ref(lua_State* L, int ref) :
				_L(L),
				_ref(ref)
			{}

			~Ref()
			{
				luaL_unref(_L, LUA_REGISTRYINDEX, _ref);
			}

			lua_State* getL() { return _L; }
			int get() { return _ref; }

		private:

			lua_State* _L;
			int _ref;

		};

		std::shared_ptr<Ref> _ref;

	};

}