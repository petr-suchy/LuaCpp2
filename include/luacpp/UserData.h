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

			lua_Integer hashCode = getHashCode(state);

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
			lua_createtable(state.getL(), 0, 0);

			// insert the garbage collector metamethod for userdata
			lua_pushcfunction(state.getL(), gc);
			// set it as field of the table
			lua_setfield(state.getL(), State::StackTop - 1, "__gc");

			// set the field with a hash code of data type
			lua_pushinteger(state.getL(), hashCode);
			lua_setfield(state.getL(), State::StackTop - 1, "hash_code");

			// set the table as metatable for the userdata
			lua_setmetatable(state.getL(), State::StackTop - 1);
		}

		// gets the hash code of userdata type
		lua_Integer getHashCode(State& state)
		{
			// insert a metatable at the top of the stack
			bool hasMetatable = lua_getmetatable(state.getL(), State::StackTop) != 0;

			if (!hasMetatable) {
				throw std::logic_error(
					"unknown userdata type"
				);
			}

			// insert hash_code at the top of the stack
			lua_getfield(
				state.getL(),
				State::StackTop,
				"hash_code"
			);

			// get the hash code
			lua_Integer hashCode = lua_tointeger(
				state.getL(),
				State::StackTop
			);

			// clear the stack
			state.pop(2);
			
			return hashCode;
		}

		static int gc(lua_State *L)
		{
			Type** userDataPtr2Ptr = reinterpret_cast<Type**>(
				lua_touserdata(L, 1)
			);

			delete *userDataPtr2Ptr;

			return 0;
		}

	};

}