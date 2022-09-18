#pragma once

#include "WritableValue.h"

namespace Lua {

	template<typename WString, typename Convert>
	class WritableWString : public WritableValue {
	public:

		WritableWString(const WString& str) :
			_str(str)
		{}

		virtual void insertTo(State& state)
		{
			state.prepareWriting();

			auto utf8Str = Convert().to_bytes(_str);

			lua_pushlstring(
				state.getL(),
				utf8Str.c_str(),
				utf8Str.length()
			);

			state.finishWriting();
		}

	private:

		const WString& _str;

	};

}