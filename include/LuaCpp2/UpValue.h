#pragma once

#include "StackModifier.h"

namespace Lua {

	class UpValue : public StackModifier {
	public:

		UpValue(int index) :
			_index(index)
		{}

		virtual void modify(State& state) const
		{
			Library::inst().pushvalue(
				state.getL(),
				Library::inst().upvalueindex(_index)
			);
		}

	private:
		int _index;
	};

}