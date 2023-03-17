#pragma once

#include <stdexcept>
#include <memory>

extern "C" {
#include "lualib.h"
#include "lauxlib.h"
}

#include "Library.h"

namespace Lua {

	class AbstractStatePointer {
	public:

		virtual bool isOpen() = 0;
		virtual Library::State* getL() = 0;

	};

	class NullStatePointer : public AbstractStatePointer {
	public:

		virtual bool isOpen()
		{
			return false;
		}

		virtual Library::State* getL()
		{
			throw std::logic_error(
				"engine state is not created"
			);

			return nullptr;
		}

	};

	class AuxStatePointer : public AbstractStatePointer {
	public:

		AuxStatePointer(Library::State* L) :
			_L(L)
		{}

		virtual bool isOpen()
		{
			return (_L != nullptr);
		}

		virtual Library::State* getL()
		{
			if (!_L) {
				throw std::logic_error(
					"invalid engine state"
				);
			}

			return _L;
		}

	private:

		Library::State* _L;

	};

	class StatePointer : public AbstractStatePointer {
	public:

		StatePointer() :
			_L(Library::inst().newstate())
		{}

		virtual ~StatePointer()
		{
			if (_L) {
				Library::inst().close(_L);
			}
		}

		virtual bool isOpen()
		{
			return (_L != nullptr);
		}

		virtual Library::State* getL()
		{
			if (!_L) {
				throw std::logic_error(
					"invalid engine state"
				);
			}

			return _L;
		}

	private:

		Library::State* _L;

	};

}