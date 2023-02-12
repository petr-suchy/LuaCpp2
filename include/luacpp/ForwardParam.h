#include "ReadableParams.h"
#include "WritableParams.h"
#include "TransferStack.h"

// overloaded stream operator for forwarding an argument

static Lua::ReadableParams operator>> (
	Lua::ReadableParams iargs,
	Lua::WritableParams oargs
)
{
	if (iargs.state().getL() != oargs.state().getL()) {

		// insert an input argument to the source stack
		iargs.prepareReading();

		// transfer the input argument from the source stack
		// to the destination stack
		oargs.prepareWriting();
		Transfer(iargs.state(), oargs.state());
		oargs.finishWriting();

		// remove the input argument
		iargs.finishReading();

	}
	else {

		// insert an input argument to the stack
		iargs.prepareReading();

		// set the input argument at the top of the stack as an output argument
		oargs.prepareWriting();
		oargs.finishWriting();

		// remove the input argument
		iargs.finishReading();

	}

	return iargs;
}

static Lua::WritableParams operator<< (
	Lua::WritableParams oargs,
	Lua::ReadableParams iargs
)
{
	iargs >> oargs;
	return oargs;
}