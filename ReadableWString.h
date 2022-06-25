#pragma once

#include "ReadableValue.h"

namespace Lua {

	template<typename WString, typename Convert>
	class ReadableWString : public ReadableValue {
	public:

		ReadableWString(WString& str) :
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

			std::string utf8Str(cstr, len);

			_str = Convert().from_bytes(utf8Str);

			state.finishReading();
		}

	private:

		WString& _str;

	};

}