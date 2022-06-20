#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "Table.h"
#include "Fields.h"
#include "Members.h"

// overloaded stream operators for reading and writing
// structured arguments

#define declhost_struct(TYPE, ...) \
static Lua::ReadableParams& operator>> ( \
	Lua::ReadableParams& args, \
	TYPE& m \
) \
{ \
	args.prepareReading(); \
\
	args.state() >> Lua::Table() \
		>> Lua::Members(#__VA_ARGS__); \
\
		args >> __VA_ARGS__; \
\
	args.state() >> Lua::Table::End(); \
\
	args.finishReading(); \
\
	return args; \
} \
\
static Lua::ReadableParams& operator, ( \
	Lua::ReadableParams& args, \
	TYPE& m \
) \
{ \
	args >> m; \
	return args; \
} \
\
static Lua::WritableParams& operator<< ( \
	Lua::WritableParams& args, \
	TYPE& m \
) \
{ \
	args.prepareWriting(); \
\
	args.state() << Lua::Table() \
		<< Lua::Members(#__VA_ARGS__); \
\
		args << __VA_ARGS__; \
\
	args.state() << Lua::Table::End(); \
\
	args.finishWriting(); \
\
	return args; \
} \
\
static Lua::WritableParams& operator, ( \
	Lua::WritableParams& args, \
	TYPE& m \
) \
{ \
	args << m; \
	return args; \
}
