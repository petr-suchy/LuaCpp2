#pragma once

#include "ReadableValue.h"
#include "ToString.h"

namespace Lua {

	template<typename String>
	class ReadableString : public ReadableValue {
	public:

		ReadableString(String& str) :
			_str(str)
		{}

		virtual void getFrom(State& state)
		{
			state.prepareReading();
			_str = ToString<String>(state, state.getStackTop());
			state.finishReading();
		}

	private:

		String& _str;

	};

}