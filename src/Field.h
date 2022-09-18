#pragma once

#include "ReadableValue.h"
#include "WritableValue.h"

namespace Lua {

	class Field : public ReadableValue, public WritableValue {
	public:

		Field(const std::string& key) :
			_key(key)
		{}

		virtual void insertTo(State& state)
		{
			state.keys().push_front(_key);
		}

		virtual void getFrom(State& state)
		{
			state.keys().push_front(_key);
		}

	private:

		std::string _key;

	};

}