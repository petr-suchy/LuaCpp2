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

			slot.prepare(Library::Type::Userdata);

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
			Library::CFunction gc,
			size_t hashCode
		) const
		{
			// insert new table at top of the stack
			state.createTable();

			// insert the garbage collector metamethod for userdata
			state.pushCFunction(gc);
			// set it as field of the table
			state.setFieldAt(State::StackTop - 1, "__gc");

			// set the field with a hash code of data type
			state.pushInteger(hashCode);
			state.setFieldAt(State::StackTop - 1, "hash_code");

			// set the table as metatable for the userdata
			state.setMetatableAt(State::StackTop - 1);
		}

		// gets the hash code of userdata type
		Library::Integer getHashCode(State& state)
		{
			// insert a metatable at the top of the stack

			if (!state.getMetatableAt(State::StackTop)) {
				throw std::logic_error(
					"unknown userdata type"
				);
			}

			// insert hash_code at the top of the stack
			state.getFieldAt(State::StackTop, "hash_code");

			// get the hash code
			Library::Integer hashCode = state.toIntegerAt(State::StackTop);

			// clear the stack
			state.pop(2);
			
			return hashCode;
		}

		static int gc(Library::State *L)
		{
			// get the first argument with userdata that contains
			// a pointer to a pointer of the given type
			auto ptrToPtr = reinterpret_cast<Type**>(
				Library::inst().touserdata(L, 1)
			);

			// and delete the pointer referenced by that pointer
			delete *ptrToPtr;

			return 0;
		}

	};

}