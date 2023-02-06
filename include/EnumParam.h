#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

// overloaded stream operators for reading and writing
// enum arguments

#define defhost_enum(TYPE) \
static Lua::ReadableParams& operator>> ( \
	Lua::ReadableParams& args, \
	TYPE& e \
) \
{ \
	std::underlying_type<TYPE>::type tmp; \
	args >> tmp; \
	e = static_cast<TYPE>(tmp); \
	return args; \
} \
static Lua::ReadableParams& operator, ( \
	Lua::ReadableParams& args, \
	TYPE& e \
) \
{ \
	args >> e; \
	return args; \
} \
\
static Lua::WritableParams& operator<< ( \
	Lua::WritableParams& args, \
	TYPE e \
) \
{ \
	args << static_cast<std::underlying_type<TYPE>::type>(e); \
	return args; \
} \
static Lua::WritableParams& operator, ( \
	Lua::WritableParams& args, \
	TYPE e \
) \
{ \
	args << e; \
	return args; \
}