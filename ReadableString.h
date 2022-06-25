#pragma once

#include "ReadableValue.h"

namespace Lua {

	template<typename String>
	class ReadableString : public ReadableValue {
	public:

		ReadableString(String& str) :
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

			_str = String(cstr, len);

			state.finishReading();
		}

	private:

		String& _str;

	};

}