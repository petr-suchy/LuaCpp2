#pragma once

#include "AbstractPointer.h"
#include "UserData.h"

namespace Lua {

	template<typename T>
	class Pointer : public AbstractPointer {
	public:

		Pointer()
		{}

		Pointer(T ptr) :
			_ptr(ptr)
		{}

		T get() { return _ptr; }

		virtual void getFrom(State& state)
		{
			UserData<EngineScope> userData;
			state >> userData;
			_ptr = userData.ptr()->ptr();
		}

		virtual void insertTo(State& state) const
		{
			state << UserData<EngineScope>(
				new EngineScope(_ptr)
			);
		}

	private:

		class EngineScope {
		public:

			EngineScope(T ptr) :
				_ptr(ptr)
			{}

			T ptr() { return _ptr; }

		private:

			T _ptr;

		};

		T _ptr;

	};

}