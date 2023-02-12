#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "ReadableString.h"
#include "WritableString.h"

#include "WritableWString.h"
#include "ReadableWString.h"

// overloaded stream operators for reading and writing
// string arguments

static Lua::WritableParams operator<< (
	Lua::WritableParams args,
	const char* cstr
)
{
	args.prepareWriting();
	args.state() << Lua::WritableString<std::string>(cstr);
	args.finishWriting();

	return args;
}

static Lua::WritableParams operator, (
	Lua::WritableParams args,
	const char* cstr
)
{
	args << cstr;
	return args;
}

#define defhost_string(STRING) \
static Lua::ReadableParams operator>> ( \
	Lua::ReadableParams args, \
	STRING& str \
) \
{ \
	args.prepareReading(); \
	Lua::ReadableString<STRING> rs(str); \
	args.state() >> rs; \
	args.finishReading(); \
\
	return args; \
} \
\
static Lua::ReadableParams operator, ( \
	Lua::ReadableParams args, \
	STRING& str \
) \
{ \
	args >> str; \
	return args; \
} \
\
static Lua::WritableParams operator<< ( \
	Lua::WritableParams args, \
	const STRING& str \
) \
{ \
	args.prepareWriting(); \
	args.state() << Lua::WritableString<STRING>(str); \
	args.finishWriting(); \
\
	return args; \
} \
\
static Lua::WritableParams operator, ( \
	Lua::WritableParams args, \
	const STRING& str \
) \
{ \
	args << str; \
	return args; \
}

#define defhost_wstring(WSTRING, CONVERT) \
static Lua::ReadableParams operator>> ( \
	Lua::ReadableParams args, \
	WSTRING& str \
) \
{ \
	args.prepareReading(); \
	Lua::ReadableWString<WSTRING, CONVERT> rws(str); \
	args.state() >> rws; \
	args.finishReading(); \
\
	return args; \
} \
\
static Lua::ReadableParams operator, ( \
	Lua::ReadableParams args, \
	WSTRING& str \
) \
{ \
	args >> str; \
	return args; \
} \
\
static Lua::WritableParams operator<< ( \
	Lua::WritableParams args, \
	const WSTRING& str \
) \
{ \
	args.prepareWriting(); \
	args.state() << Lua::WritableWString<WSTRING, CONVERT>(str); \
	args.finishWriting(); \
\
	return args; \
} \
\
static Lua::WritableParams operator, ( \
	Lua::WritableParams args, \
	const WSTRING& str \
) \
{ \
	args << str; \
	return args; \
}
