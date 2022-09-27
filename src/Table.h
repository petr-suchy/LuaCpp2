#pragma once

#include "ReadableValue.h"
#include "WritableValue.h"

namespace Lua {

	class Table : public ReadableValue, public WritableValue {
	public:

		class End : public ReadableValue, public WritableValue {
		public:

			virtual void getFrom(State& state)
			{
				state.leaveTable();
				state.finishReading();
			}

			virtual void insertTo(State& state)
			{
				state.leaveTable();

				// nested table is set as a field of its parent table

				if (state.getTableLevel() > 0) {
					state.setValueAsField();
				}

				// the top table is leaved in the stack
			}

		};

		virtual void getFrom(State& state)
		{
			state.prepareReading(LUA_TTABLE);
			state.enterTable();
		}

		virtual void insertTo(State& state)
		{
			state.prepareWriting();
			state.enterTable();

			lua_createtable(state.getL(), 0, 0);
		}

	};

}

#include "X_Table.h"