#pragma once

#include "State.h"

namespace Lua {

	class ReadableParams {
	public:

		typedef std::function<int(State&)> CountFunc;
		typedef std::function<void(State&)> PrepareReadingFunc;
		typedef std::function<void(State&)> FinishReadingFunc;

		ReadableParams(
			State& state,
			CountFunc countFunc,
			PrepareReadingFunc prepareReadingFunc,
			FinishReadingFunc finishReadingFunc
		) :
			_state(state),
			_countFunc(countFunc),
			_prepareReadingFunc(prepareReadingFunc),
			_finishReadingFunc(finishReadingFunc)
		{}

		State& state() const { return _state; }
		bool atEnd() const { return count() == 0; }
		int count() const { return _countFunc(_state); }

		void prepareReading()
		{
			if (_state.isReadyForReadingParam()) {
				_prepareReadingFunc(_state);
			}
		}

		void finishReading()
		{
			if (_state.isReadyForReadingParam()) {
				_finishReadingFunc(_state);
			}
		}

	private:

		State& _state;
		CountFunc _countFunc;
		PrepareReadingFunc _prepareReadingFunc;
		FinishReadingFunc _finishReadingFunc;

	};

}