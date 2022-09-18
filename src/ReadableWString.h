#pragma once

#include "ReadableValue.h"
#include "ToString.h"

namespace Lua {

	template<typename WString, typename Convert>
	class ReadableWString : public ReadableValue {
	public:

		ReadableWString(WString& str) :
			_str(str)
		{}

		virtual void getFrom(State& state)
		{
			state.prepareReading();

			auto utf8Str = ToString<std::string>(
				state,
				state.getStackTop()
			);

			_str = Convert().from_bytes(utf8Str);

			state.finishReading();
		}

	private:

		WString& _str;

	};

}