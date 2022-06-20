#pragma once

#include "ReadableValue.h"
#include "WritableValue.h"

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
			state.prepareReading(LUA_TUSERDATA);

			void* rawUserDataPtr2Ptr = lua_touserdata(
				state.getL(),
				state.getStackTop()
			);

			Type** userDataPtr2Ptr = reinterpret_cast<Type**>(rawUserDataPtr2Ptr);

			if (_release) {
				delete _dataPtr;
				_release = false;
			}

			_dataPtr = *userDataPtr2Ptr;

			state.finishReading();
		}

		// inserts the userdata at the top of the stack
		virtual void insertTo(State& state)
		{
			state.prepareWriting();

			insertToStack(state);
			setReleaseFunc(state, UserData<Type>::gc);

			state.finishWriting();
		}

	private:

		Type* _dataPtr; // holds a pointer to the wrapped type
		bool _release;

		// create a userdata for the wrapped type pointer
		// and insert it at the top of the stack
		void insertToStack(State& state)
		{
			if (!_dataPtr) {
				throw std::logic_error("wrapped type pointer is null");
			}

			// allocate a userdata pointer to the wrapped type pointer
			// and insert it at the top of the stack as userdata
			Type** userDataPtr2Ptr =  reinterpret_cast<Type**>(
				lua_newuserdata(state.getL(), sizeof(Type*))
			);

			if (!userDataPtr2Ptr) {
				throw std::runtime_error("not enough memory");
			}

			// set the userdata pointer to the wrapped type pointer
			*userDataPtr2Ptr = _dataPtr;
			_release = false;

		}

		// sets the garbage collector method for the userdata
		// at the top of the stack
		void setReleaseFunc(State& state, lua_CFunction func)
		{
			// insert new table at top of the stack
			lua_createtable(state.getL(), 0, 0);

			// insert the garbage collector metamethod for userdata
			lua_pushcfunction(state.getL(), func);

			// set it as field of the table
			lua_setfield(state.getL(), State::StackTop - 1, "__gc");

			// set the table as metatable for the userdata
			lua_setmetatable(state.getL(), State::StackTop - 1);
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