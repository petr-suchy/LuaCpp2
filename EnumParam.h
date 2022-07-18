#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

// overloaded stream operators for reading and writing
// enum arguments

#define defhost_enum(TYPE) \
Lua::ReadableParams& operator>> (Lua::ReadableParams& args, TYPE& e) \
{ \
	std::underlying_type<TYPE>::type val; \
	args >> val; \
	e = static_cast<TYPE>(val); \
	return args; \
} \
\
static Lua::WritableParams& operator<< (Lua::WritableParams& args, TYPE val) \
{ \
	args << static_cast<std::underlying_type<TYPE>::type>(val); \
	return args; \
}
