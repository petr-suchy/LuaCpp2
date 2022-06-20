#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "Pointer.h"

// overloaded stream operators for reading and writing
// pointer arguments

static Lua::ReadableParams& operator>> (
	Lua::ReadableParams& args,
	Lua::AbstractPointer& ptr
)
{
	args.prepareReading();
	args.state() >> ptr;
	args.finishReading();

	return args;
}

static Lua::ReadableParams& operator, (
	Lua::ReadableParams& args,
	Lua::AbstractPointer& ptr
)
{
	args >> ptr;
	return args;
}

static Lua::WritableParams& operator<< (
	Lua::WritableParams& args,
	Lua::AbstractPointer& ptr
)
{
	args.prepareWriting();
	args.state() << ptr;
	args.finishWriting();

	return args;
}

static Lua::WritableParams& operator, (
	Lua::WritableParams& args,
	Lua::AbstractPointer& ptr
)
{
	args << ptr;
	return args;
}