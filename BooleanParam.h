#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "ReadableBoolean.h"
#include "WritableBoolean.h"

// overloaded stream operators for reading and writing
// boolean arguments

static Lua::ReadableParams& operator>> (
	Lua::ReadableParams& args,
	bool& val
)
{
	args.prepareReading();
	args.state() >> Lua::ReadableBoolean(val);
	args.finishReading();

	return args;
}

static Lua::ReadableParams& operator, (
	Lua::ReadableParams& args,
	bool& val
)
{
	args >> val;
	return args;
}

static Lua::WritableParams& operator<< (
	Lua::WritableParams& args,
	bool val
)
{
	args.prepareWriting();
	args.state() << Lua::WritableBoolean(val);
	args.finishWriting();

	return args;
}

static Lua::WritableParams& operator, (
	Lua::WritableParams& args,
	bool val
)
{
	args << val;
	return args;
}
