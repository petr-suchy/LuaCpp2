#pragma once

#include "State.h"

namespace Lua {

	class WritableParams {
	public:

		typedef std::function<int(State&)> CountFunc;
		typedef std::function<void(State&)> PrepareWritingFunc;
		typedef std::function<void(State&)> FinishWritingFunc;

		WritableParams(
			State& state,
			CountFunc countFunc,
			PrepareWritingFunc prepareWritingFunc,
			FinishWritingFunc finishWritingFunc
		) :
			_state(state),
			_countFunc(countFunc),
			_prepareWritingFunc(prepareWritingFunc),
			_finishWritingFunc(finishWritingFunc)
		{}

		State& state() { return _state; }
		bool atEnd() { return count() == 0; }
		int count() { return _countFunc(_state); }

		void prepareWriting()
		{
			if (_state.isReadyForWritingParam()) {
				_prepareWritingFunc(_state);
			}
		}

		void finishWriting()
		{
			if (_state.isReadyForWritingParam()) {
				_finishWritingFunc(_state);
			}
		}

	private:

		State& _state;
		CountFunc _countFunc;
		PrepareWritingFunc _prepareWritingFunc;
		FinishWritingFunc _finishWritingFunc;

	};

}