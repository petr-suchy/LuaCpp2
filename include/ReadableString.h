#pragma once

#include "ReadableValue.h"
#include "ReadableStackSlot.h"
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
			ReadableStackSlot slot(state);

			slot.prepare();
			_str = slot.getString<String>();
			slot.finish();
		}

	private:

		String& _str;

	};

}