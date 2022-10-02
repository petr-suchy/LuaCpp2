#pragma once

#include "State.h"
#include "WritableStackSlot.h"

namespace Lua {

	class OutputStack {
	public:

		OutputStack(State& state) :
			_state(state)
		{}

		void insertNil()
		{
			WritableStackSlot slot(_state);

			slot.prepare();
			slot.insertNil();
			slot.finish();
		}

		void insertBoolean(bool boolVal)
		{
			WritableStackSlot slot(_state);

			slot.prepare();
			slot.insertBoolean(boolVal);
			slot.finish();
		}

		void insertInteger(lua_Integer num)
		{
			WritableStackSlot slot(_state);

			slot.prepare();
			slot.insertInteger(num);
			slot.finish();
		}

		void insertNumber(lua_Number num)
		{
			WritableStackSlot slot(_state);

			slot.prepare();
			slot.insertNumber(num);
			slot.finish();
		}

		void insertString(const std::string& str)
		{
			WritableStackSlot slot(_state);

			slot.prepare();
			slot.insertString(str);
			slot.finish();
		}

		void insertTable()
		{
			WritableStackSlot slot(_state);

			slot.prepare();
			slot.insertTable();
			slot.finish();
		}

		void setTable()
		{
			lua_settable(_state.getL(), -3);
		}

	private:

		State& _state;

	};

}