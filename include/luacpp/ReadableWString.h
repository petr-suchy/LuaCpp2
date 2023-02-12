#pragma once

#include "ReadableValue.h"
#include "ReadableStackSlot.h"
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
			ReadableStackSlot slot(state);

			slot.prepare();

			auto utf8Str = slot.getString<std::string>();
			_str = Convert().from_bytes(utf8Str);

			slot.finish();
		}

	private:

		WString& _str;

	};

}