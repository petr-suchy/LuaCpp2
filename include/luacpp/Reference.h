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

			int ref = slot.getReference();

			if (ref == LUA_REFNIL) {
				throw std::runtime_error("nil reference returned");
			}

			_ref = std::make_shared<Ref>(state.getL(), ref);

			slot.finish();
		}

		virtual void insertTo(State& state) const
		{
			if (state.getL() != _ref->getL()) {
				throw std::logic_error("invalid reference");
			}

			WritableStackSlot slot(state);

			slot.prepare();
			slot.insertReference(_ref->get());
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