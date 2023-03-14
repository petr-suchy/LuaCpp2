#pragma once

#include "FunctionImpl.h"
#include "Closure.h"
#include "UpValue.h"
#include "Pointer.h"

namespace Lua {

	// implementation for a host function
	class HostFunctionImpl : public FunctionImpl {
	public:

		// prototype of the host function
		typedef std::function<int(State&)> FunctionPrototype;

		HostFunctionImpl(const FunctionPrototype& func) :
			_funcPtr(std::make_shared<FunctionPrototype>(func))
		{}

		// gets a weak pointer to the engine state
		virtual State::WeakPtr getWeakStatePtr()
		{
			throw std::logic_error(
				"host function has no weak pointer to the engine state"
			);

			return State::SharedPtr();
		}

		// inserts the host function to the stack
		void insertTo(State& state) const
		{
			Lua::WritableStackSlot slot(state);

			slot.prepare();
			{
				Lua::Closure closure(slot, callFunc);

				closure.upValues() << _funcPtr;
				closure.insert();
			}
			slot.finish();
		}

	private:

		// shared pointer to the host function
		typedef Lua::Pointer<
			std::shared_ptr<FunctionPrototype>
		> FunctionPointer;

		// calls the host function
		static int callFunc(Library::State* L)
		{
			State state(L);
			int numOfOutputArgs = 0;
			
			try {

				FunctionPointer funcPtr;

				state >> UpValue(1) >> funcPtr;

				numOfOutputArgs = (*funcPtr.get())(state);

			}
			catch (std::exception& e) {
				state.raiseError(e.what());
			}
			catch (...) {
				state.raiseError("unknown exception");
			}

			// adjust the top of the stack to the number of output arguments
			state.setStackTop(numOfOutputArgs);

			return numOfOutputArgs;
		}

		// holds a pointer to the host function
		FunctionPointer _funcPtr;

	};

}