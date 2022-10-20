#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

// overloaded stream operator for ignoring an argument

namespace Lua {
	class IgnoreParam {};
}

static Lua::ReadableParams& operator>> (
	Lua::ReadableParams& args,
	Lua::IgnoreParam
)
{
	args.prepareReading();
	args.state().pop();
	args.finishReading();

	return args;
}