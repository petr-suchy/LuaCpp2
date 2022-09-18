#pragma once

#include "ReadableValue.h"
#include "WritableValue.h"

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
			state.prepareReading();

			// pops a value from the stack, stores it into
			// the registry with a fresh integer key, and returns
			// that key as "reference"
			int ref = luaL_ref(state.getL(), LUA_REGISTRYINDEX);

			_ref = std::make_shared<Ref>(state.getL(), ref);

			state.noRemoval();
			state.finishReading();
		}

		virtual void insertTo(State& state)
		{
			if (state.getL() != _ref->getL()) {
				throw std::logic_error("invalid reference");
			}

			// pushes a value associated with the reference
			// onto the stack
			lua_rawgeti(state.getL(), LUA_REGISTRYINDEX, _ref->get());
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