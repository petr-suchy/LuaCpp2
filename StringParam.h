#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "ReadableString.h"
#include "WritableString.h"

// overloaded stream operators for reading and writing
// string arguments

static Lua::ReadableParams& operator>> (
	Lua::ReadableParams& args,
	std::string& str
)
{
	args.prepareReading();
	args.state() >> Lua::ReadableString(str);
	args.finishReading();

	return args;
}

static Lua::ReadableParams& operator, (
	Lua::ReadableParams& args,
	std::string& str
)
{
	args >> str;
	return args;
}

static Lua::WritableParams& operator<< (
	Lua::WritableParams& args,
	const std::string& str
)
{
	args.prepareWriting();
	args.state() << Lua::WritableString(str);
	args.finishWriting();

	return args;
}

static Lua::WritableParams& operator, (
	Lua::WritableParams& args,
	const std::string& str
)
{
	args << str;
	return args;
}

