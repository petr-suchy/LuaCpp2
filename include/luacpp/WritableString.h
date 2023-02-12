#pragma once

#include "WritableValue.h"
#include "WritableStackSlot.h"

namespace Lua {

	template<typename String>
	class WritableString : public WritableValue {
	public:

		WritableString(const String& str) :
			_str(str)
		{}

		virtual void insertTo(State& state) const
		{
			WritableStackSlot slot(state);
			
			slot.prepare();
			slot.insertString(_str);
			slot.finish();
		}

	private:

		const String& _str;

	};

}