#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "ReadableValue.h"
#include "WritableValue.h"

static Lua::ReadableParams operator>> (
	Lua::ReadableParams args,
	Lua::ReadableValue& val
)
{
	args.prepareReading();
	args.state() >> val;
	args.finishReading();

	return args;
}

static Lua::ReadableParams operator, (
	Lua::ReadableParams args,
	Lua::ReadableValue& val
)
{
	args >> val;
	return args;
}

static Lua::WritableParams operator<< (
	Lua::WritableParams args,
	Lua::WritableValue& val
)
{
	args.prepareWriting();
	args.state() << val;
	args.finishWriting();

	return args;
}

static Lua::WritableParams operator, (
	Lua::WritableParams args,
	Lua::WritableValue& val
)
{
	args << val;
	return args;
}