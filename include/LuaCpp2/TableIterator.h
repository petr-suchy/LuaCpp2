#pragma once

#include "State.h"

namespace Lua {

	template<typename Item>
	class TableIterator {
	public:

		TableIterator(State& state, int tableIndex) :
			_state(state),
			_tableIndex(tableIndex),
			_isFirstIteration(true),
			_atEnd(false),
			_key(_state),
			_val(state)
		{}

		~TableIterator()
		{
			if (!_isFirstIteration && !_atEnd) {
				// remove both the key and the value left on the stack
				// from the last iteration
				_state.pop(2);
			}
		}

		State& state() const { return _state; }
		int getIndex() const { return _tableIndex; }
		bool atEnd() const { return _atEnd; }
		const Item& key() const { return _key; }
		const Item& val() const { return _val; }

		void first()
		{
			if (!_isFirstIteration) {

				if (!_atEnd) {
					// remove both the key and the value left on the stack
					// from the last iteration
					_state.pop(2);
				}

				_isFirstIteration = true;
				_atEnd = false;
			}

			next();
		}

		void next()
		{
			if (_atEnd) {
				return;
			}

			if (_isFirstIteration) {
				// push the initialization key for the first iteration
				Library::inst().pushnil(_state.getL());
				_isFirstIteration = false;
			}
			else {
				// remove the value from the previous iteration
				// the key is left on the stack for the next iteration
				_state.pop();
			}

			// remove the key from the stack, and pushes
			// a key-value pair from the table
			int result = Library::inst().next(_state.getL(), _tableIndex);

			if (!result) {
				_atEnd = true;
				return;
			}
			
			int top = _state.getStackTop();
			
			int keyIndex = top - 1;
			int valIndex = top;

			_key.setIndex(keyIndex);
			_val.setIndex(valIndex);
			
		}

	private:

		State& _state;
		int _tableIndex;
		bool _isFirstIteration;
		bool _atEnd;

		Item _key;
		Item _val;

	};

}