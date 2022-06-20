#pragma once

#include "WritableValue.h"

namespace Lua {

	class WritableString : public WritableValue {
	public:

		WritableString(const std::string& str) :
			_str(str)
		{}

		virtual void insertTo(State& state)
		{
			state.prepareWriting();
			lua_pushlstring(state.getL(), _str.c_str(), _str.length());
			state.finishWriting();
		}

	private:

		const std::string& _str;

	};

}