#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "ReadableNumber.h"
#include "WritableNumber.h"

// overloaded stream operators for reading and writing
// numeric arguments

static Lua::ReadableParams& operator>> (
	Lua::ReadableParams& args,
	int& num
)
{
	args.prepareReading();
	args.state() >> Lua::ReadableNumber(num);
	args.finishReading();

	return args;
}

static Lua::ReadableParams& operator, (
	Lua::ReadableParams& args,
	int& num
)
{
	args >> num;
	return args;
}

static Lua::WritableParams& operator<< (
	Lua::WritableParams& args,
	int num
)
{
	args.prepareWriting();
	args.state() << Lua::WritableNumber(num);
	args.finishWriting();

	return args;
}

static Lua::WritableParams& operator, (
	Lua::WritableParams& args,
	int num
)
{
	args << num;
	return args;
}