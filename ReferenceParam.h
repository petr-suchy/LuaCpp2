#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "Reference.h"

// overloaded stream operators for reading and writing
// reference arguments

static Lua::ReadableParams& operator>> (
	Lua::ReadableParams& args,
	Lua::Reference& ref
)
{
	args.prepareReading();
	args.state() >> ref;
	args.finishReading();

	return args;
}

static Lua::ReadableParams& operator, (
	Lua::ReadableParams& args,
	Lua::Reference& ref
)
{
	args >> ref;
	return args;
}

static Lua::WritableParams& operator<< (
	Lua::WritableParams& args,
	Lua::Reference& ref
)
{
	args.prepareWriting();
	args.state() << ref;
	args.finishWriting();

	return args;
}

static Lua::WritableParams& operator, (
	Lua::WritableParams& args,
	Lua::Reference& ref
)
{
	args << ref;
	return args;
}