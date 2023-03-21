#pragma once

#include "State.h"
#include "WritableStackSlot.h"

namespace Lua {

	class OutputStack {
	public:

		OutputStack(State& state) :
			_state(state),
			_top(state.getStackTop()),
			_finished(false)
		{}

		~OutputStack()
		{
			if (!_finished) {
				_state.setStackTop(_top);
			}
		}

		void finish()
		{
			_finished = true;
		}

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

		void insertInteger(Library::Integer num)
		{
			WritableStackSlot slot(_state);

			slot.prepare();
			slot.insertInteger(num);
			slot.finish();
		}

		void insertNumber(Library::Number num)
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
			_state.setFieldAt(State::StackTop - 2);
		}

	private:

		State& _state;
		int _top;
		bool _finished;

	};

}