#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "ReadableString.h"
#include "WritableString.h"

#include "WritableWString.h"
#include "ReadableWString.h"

#include <codecvt>

// overloaded stream operators for reading and writing
// string arguments

#define defhost_string(STRING) \
static Lua::ReadableParams& operator>> ( \
	Lua::ReadableParams& args, \
	STRING& str \
) \
{ \
	args.prepareReading(); \
	args.state() >> Lua::ReadableString<STRING>(str); \
	args.finishReading(); \
\
	return args; \
} \
\
static Lua::ReadableParams& operator, ( \
	Lua::ReadableParams& args, \
	STRING& str \
) \
{ \
	args >> str; \
	return args; \
} \
\
static Lua::WritableParams& operator<< ( \
	Lua::WritableParams& args, \
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
static Lua::WritableParams& operator, ( \
	Lua::WritableParams& args, \
	const STRING& str \
) \
{ \
	args << str; \
	return args; \
}

#define defhost_wstring(WSTRING, CONVERT) \
static Lua::ReadableParams& operator>> ( \
	Lua::ReadableParams& args, \
	WSTRING& str \
) \
{ \
	args.prepareReading(); \
	args.state() >> Lua::ReadableWString<WSTRING, CONVERT>(str); \
	args.finishReading(); \
\
	return args; \
} \
\
static Lua::ReadableParams& operator, ( \
	Lua::ReadableParams& args, \
	WSTRING& str \
) \
{ \
	args >> str; \
	return args; \
} \
\
static Lua::WritableParams& operator<< ( \
	Lua::WritableParams& args, \
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
static Lua::WritableParams& operator, ( \
	Lua::WritableParams& args, \
	const WSTRING& str \
) \
{ \
	args << str; \
	return args; \
}

defhost_wstring(
	std::wstring,
	std::wstring_convert<
		std::codecvt_utf8<wchar_t>
	>
)

defhost_string(std::string)
