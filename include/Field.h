#pragma once

#include "StackModifier.h"

namespace Lua {

	class Field : public StackModifier {
	public:

		Field(const std::string& key) :
			_key(key)
		{}

		virtual void modify(State& state) const
		{
			state.keys().push_front(_key);
		}

	private:

		std::string _key;

	};

}