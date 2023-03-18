#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "Table.h"
#include "Fields.h"
#include "Members.h"

// overloaded stream operators for reading and writing
// structured arguments

#define defhost_struct(TYPE, ...) \
static Lua::ReadableParams operator>> ( \
	Lua::ReadableParams args, \
	TYPE& m \
) \
{ \
	args.prepareReading(); \
	{ \
		Lua::ReadableStackSlot slot(args.state()); \
\
		slot.prepare(Lua::Library::Type::Table); \
		{ \
			Lua::Table tbl(slot.getTable()); \
			tbl.state() >> Lua::Members(#__VA_ARGS__); \
			args >> __VA_ARGS__; \
		} \
		slot.finish(); \
	} \
	args.finishReading(); \
\
	return args; \
} \
\
static Lua::ReadableParams operator, ( \
	Lua::ReadableParams args, \
	TYPE& m \
) \
{ \
	args >> m; \
	return args; \
} \
\
static Lua::WritableParams operator<< ( \
	Lua::WritableParams args, \
	const TYPE& m \
) \
{ \
	args.prepareWriting(); \
	{ \
		Lua::WritableStackSlot slot(args.state()); \
\
		slot.prepare(); \
		{ \
			Lua::Table tbl(slot.insertTable()); \
			tbl.state() << Lua::Members(#__VA_ARGS__); \
			args << __VA_ARGS__; \
		} \
		slot.finish(); \
	} \
	args.finishWriting(); \
\
	return args; \
} \
\
static Lua::WritableParams operator, ( \
	Lua::WritableParams args, \
	const TYPE& m \
) \
{ \
	args << m; \
	return args; \
}
