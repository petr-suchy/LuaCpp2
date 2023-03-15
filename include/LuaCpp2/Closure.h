#pragma once

#include "WritableStackSlot.h"

namespace Lua {

	class Closure {
	public:

		Closure(WritableStackSlot& slot, Library::CFunction func) :
			_slot(slot),
			_func(func),
			_top(slot.state().getStackTop()),
			_tableLevel(0),
			_inserted(false)
		{
			_slot.state().enterClosure(_tableLevel);
		}

		~Closure()
		{
			_slot.state().setStackTop(_top);
			_slot.state().leaveClosure(_tableLevel);
		}

		State& upValues()
		{
			return _slot.state();
		}

		void insert()
		{
			if (!_inserted) {
				int numOfUpValues = _slot.state().getStackTop() - _top;
				_slot.insertClosure(_func, numOfUpValues);
				_top = _slot.state().getStackTop();
				_inserted = true;
			}
		}

	private:

		WritableStackSlot& _slot;
		Library::CFunction _func;
		int _top;
		int _tableLevel;
		bool _inserted;

	};

}