#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "Table.h"
#include "Fields.h"
#include "Members.h"

// overloaded stream operators for reading and writing
// read-only structured arguments

#define defhost_const_struct(TYPE, ...) \
static Lua::ReadableParams& operator>> ( \
	Lua::ReadableParams& args, \
	const TYPE& m \
) \
{ \
	args.prepareReading(); \
\
	args.state() >> Lua::Table() \
		>> Lua::Table::End(); \
\
	args.finishReading(); \
\
	return args; \
} \
static Lua::ReadableParams& operator, ( \
	Lua::ReadableParams& args, \
	const TYPE& m \
) \
{ \
	args >> m; \
	return args; \
} \
\
static Lua::WritableParams& operator<< ( \
	Lua::WritableParams& args, \
	const TYPE& m \
) \
{ \
	args.prepareWriting(); \
	args.state() << Lua::Table() \
\
		<< Lua::Field("__index") << Lua::Table() \
			<< Lua::Members(#__VA_ARGS__); \
			args << __VA_ARGS__; \
		args.state() << Lua::Table::End(); \
\
		args.state() << Lua::Field("__newindex") \
		<< Lua::Function( \
			[] (Lua::State&) \
			{ \
				throw std::logic_error("attempt to change a read-only object"); \
				return 0; \
			} \
		); \
\
	args.state() << Lua::Table::End(); \
	args.finishWriting(); \
\
	return args; \
} \
\
static Lua::WritableParams& operator, ( \
	Lua::WritableParams& args, \
	const TYPE& m \
) \
{ \
	args << m; \
	return args; \
}
