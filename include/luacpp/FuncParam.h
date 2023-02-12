#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "Function.h"

// overloaded stream operators for reading and writing
// functional arguments

static Lua::ReadableParams operator>> (
	Lua::ReadableParams args,
	Lua::Function& func
)
{
	args.prepareReading();
	args.state() >> func;
	args.finishReading();

	return args;
}

static Lua::ReadableParams operator, (
	Lua::ReadableParams args,
	Lua::Function& func
)
{
	args >> func;
	return args;
}

static const Lua::WritableParams operator<< (
	Lua::WritableParams args,
	const Lua::AbstractFunction& func
)
{
	args.prepareWriting();
	args.state() << func;
	args.finishWriting();

	return args;
}

static const Lua::WritableParams operator, (
	Lua::WritableParams args,
	const Lua::AbstractFunction& func
)
{
	args << func;
	return args;
}