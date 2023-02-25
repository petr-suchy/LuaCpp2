#pragma once

#include <stdexcept>
#include <memory>

extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}

namespace Lua {

	class AbstractStatePointer {
	public:

		virtual bool isOpen() = 0;
		virtual lua_State* getL() = 0;

	};

	class NullStatePointer : public AbstractStatePointer {
	public:

		virtual bool isOpen()
		{
			return false;
		}

		virtual lua_State* getL()
		{
			throw std::logic_error(
				"engine state is not created"
			);

			return nullptr;
		}

	};

	class AuxStatePointer : public AbstractStatePointer {
	public:

		AuxStatePointer(lua_State* L) :
			_L(L)
		{}

		virtual bool isOpen()
		{
			return (_L != nullptr);
		}

		virtual lua_State* getL()
		{
			if (!_L) {
				throw std::logic_error(
					"invalid engine state"
				);
			}

			return _L;
		}

	private:

		lua_State* _L;

	};

	class StatePointer : public AbstractStatePointer {
	public:

		StatePointer() :
			_L(luaL_newstate())
		{}

		virtual ~StatePointer()
		{
			if (_L) {
				lua_close(_L);
			}
		}

		virtual bool isOpen()
		{
			return (_L != nullptr);
		}

		virtual lua_State* getL()
		{
			if (!_L) {
				throw std::logic_error(
					"invalid engine state"
				);
			}

			return _L;
		}

	private:

		lua_State* _L;

	};

}