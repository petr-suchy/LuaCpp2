#pragma once

#include "ReadableValue.h"
#include "WritableValue.h"
#include "ReadableStackSlot.h"
#include "WritableStackSlot.h"

#include <typeinfo>

namespace Lua {

	template<typename WrappedType>
	class UserData : public ReadableValue, public WritableValue {
	public:

		// wrapped type
		typedef WrappedType Type;

		UserData(Type* dataPtr = nullptr) :
			_dataPtr(dataPtr),
			_release(dataPtr != nullptr)
		{}

		~UserData()
		{
			if (_release) {
				delete _dataPtr;
			}
		}

		// gets a pointer to the wrapped type
		Type* ptr() { return _dataPtr; }

		// gets the userdata from the top of the stack
		virtual void getFrom(State& state)
		{
			ReadableStackSlot slot(state);

			slot.prepare(LUA_TUSERDATA);

			auto dataPtr = slot.getUserData<Type>();

			Library::Integer hashCode = getHashCode(state);

			if (hashCode != typeid(WrappedType).hash_code()) {
				throw std::logic_error(
					"invalid userdata type"
				);
			}

			if (_release) {
				delete _dataPtr;
				_release = false;
			}

			_dataPtr = dataPtr;
			slot.finish();
		}

		// inserts the userdata at the top of the stack
		virtual void insertTo(State& state) const
		{
			if (!_dataPtr) {
				throw std::logic_error("wrapped type pointer is null");
			}

			WritableStackSlot slot(state);
			
			slot.prepare();

			slot.insertUserData(_dataPtr);
			_release = false;

			setMetaInfo(
				slot.state(),
				UserData<Type>::gc,
				typeid(WrappedType).hash_code()
			);

			slot.finish();
		}

	private:

		Type* _dataPtr; // holds a pointer to the wrapped type
		mutable bool _release; // TODO: replace with reference counter

		// sets meta information about userdata at the top of the stack
		void setMetaInfo(
			State& state,
			lua_CFunction gc,
			size_t hashCode
		) const
		{
			// insert new table at top of the stack
			Library::inst().createtable(state.getL(), 0, 0);

			// insert the garbage collector metamethod for userdata
			Library::inst().pushcfunction(state.getL(), gc);
			// set it as field of the table
			Library::inst().setfield(state.getL(), State::StackTop - 1, "__gc");

			// set the field with a hash code of data type
			Library::inst().pushinteger(state.getL(), hashCode);
			Library::inst().setfield(state.getL(), State::StackTop - 1, "hash_code");

			// set the table as metatable for the userdata
			Library::inst().setmetatable(state.getL(), State::StackTop - 1);
		}

		// gets the hash code of userdata type
		Library::Integer getHashCode(State& state)
		{
			// insert a metatable at the top of the stack
			bool hasMetatable = Library::inst().getmetatable(state.getL(), State::StackTop) != 0;

			if (!hasMetatable) {
				throw std::logic_error(
					"unknown userdata type"
				);
			}

			// insert hash_code at the top of the stack
			Library::inst().getfield(
				state.getL(),
				State::StackTop,
				"hash_code"
			);

			// get the hash code
			Library::Integer hashCode = Library::inst().tointeger(
				state.getL(),
				State::StackTop
			);

			// clear the stack
			state.pop(2);
			
			return hashCode;
		}

		static int gc(Library::State *L)
		{
			Type** userDataPtr2Ptr = reinterpret_cast<Type**>(
				lua_touserdata(L, 1)
			);

			delete *userDataPtr2Ptr;

			return 0;
		}

	};

}