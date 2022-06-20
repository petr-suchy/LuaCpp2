#pragma once

#include "ReadableValue.h"

namespace Lua {

	class ReadableString : public ReadableValue {
	public:

		ReadableString(std::string& str) :
			_str(str)
		{}

		virtual void getFrom(State& state)
		{
			state.prepareReading(LUA_TSTRING);

			size_t len = 0;

			const char* cstr = lua_tolstring(
				state.getL(),
				state.getStackTop(),
				&len
			);

			_str = std::string(cstr, len);

			state.finishReading();
		}

	private:

		std::string& _str;

	};

}