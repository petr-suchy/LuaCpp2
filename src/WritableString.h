#pragma once

#include "WritableValue.h"

namespace Lua {

	template<typename String>
	class WritableString : public WritableValue {
	public:

		WritableString(const String& str) :
			_str(str)
		{}

		virtual void insertTo(State& state)
		{
			state.prepareWriting();
			lua_pushlstring(state.getL(), _str.c_str(), _str.length());
			state.finishWriting();
		}

	private:

		const String& _str;

	};

}