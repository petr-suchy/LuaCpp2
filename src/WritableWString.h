#pragma once

#include "WritableValue.h"
#include "WritableStackSlot.h"

namespace Lua {

	template<typename WString, typename Convert>
	class WritableWString : public WritableValue {
	public:

		WritableWString(const WString& str) :
			_str(str)
		{}

		virtual void insertTo(State& state)
		{
			auto utf8Str = Convert().to_bytes(_str);

			WritableStackSlot slot(state);
			
			slot.prepare();
			slot.insertString(utf8Str);
			slot.finish();
		}

	private:

		const WString& _str;

	};

}