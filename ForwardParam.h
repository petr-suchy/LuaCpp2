#include "ReadableParams.h"
#include "WritableParams.h"

// overloaded stream operator for forwarding an argument

static Lua::ReadableParams& operator>> (
	Lua::ReadableParams& iargs,
	Lua::WritableParams& oargs
)
{
	// insert an input argument to the stack
	iargs.prepareReading();

	// set the input argument at the top of the stack as an output argument
	oargs.prepareWriting();
	oargs.finishWriting();

	// remove the input argument
	iargs.finishReading();

	return iargs;
}

static Lua::WritableParams& operator<< (
	Lua::WritableParams& oargs,
	Lua::ReadableParams& iargs
)
{
	iargs >> oargs;
	return oargs;
}