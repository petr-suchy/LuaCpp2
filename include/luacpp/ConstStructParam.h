#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "Table.h"
#include "Fields.h"
#include "Members.h"

// overloaded stream operators for reading and writing
// read-only structured arguments

#define defhost_const_struct(TYPE, ...) \
static Lua::ReadableParams operator>> ( \
	Lua::ReadableParams args, \
	const TYPE& m \
) \
{ \
	args.prepareReading(); \
	{ \
		Lua::ReadableStackSlot slot(args.state()); \
\
		slot.prepare(LUA_TTABLE); \
		{ \
			Lua::Table tbl(slot.getTable()); \
		} \
		slot.finish(); \
	} \
	args.finishReading(); \
\
	return args; \
} \
static Lua::ReadableParams operator, ( \
	Lua::ReadableParams args, \
	const TYPE& m \
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
\
			tbl.state() << Lua::Field("__index"); \
			{ \
				Lua::WritableStackSlot slot2(tbl.state()); \
\
				slot2.prepare(); \
				{ \
					Lua::Table tbl2(slot2.insertTable()); \
					tbl2.state() << Lua::Members(#__VA_ARGS__); \
					args << __VA_ARGS__; \
				} \
				slot2.finish(); \
			} \
			\
			tbl.state() << Lua::Field("__newindex") \
			<< Lua::Function( \
				[] (Lua::State&) \
				{ \
					throw std::logic_error("attempt to change a read-only object"); \
					return 0; \
				} \
			); \
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
