#pragma once

#include "ReadableParams.h"
#include "WritableParams.h"

#include "ReadableNumber.h"
#include "WritableNumber.h"
#include "ReadableInteger.h"
#include "WritableInteger.h"

// overloaded stream operators for reading and writing
// numeric arguments

#define defhost_number(TYPE) \
static Lua::ReadableParams& operator>> ( \
	Lua::ReadableParams& args, \
	TYPE& num \
) \
{ \
	args.prepareReading(); \
	args.state() >> Lua::ReadableNumber<TYPE>(num); \
	args.finishReading(); \
\
	return args; \
} \
\
static Lua::ReadableParams& operator, ( \
	Lua::ReadableParams& args, \
	TYPE& num \
) \
{ \
	args >> num; \
	return args; \
} \
\
static Lua::WritableParams& operator<< ( \
	Lua::WritableParams& args, \
	TYPE num \
) \
{ \
	args.prepareWriting(); \
	args.state() << Lua::WritableNumber<TYPE>(num); \
	args.finishWriting(); \
\
	return args; \
} \
\
static Lua::WritableParams& operator, ( \
	Lua::WritableParams& args, \
	TYPE num \
) \
{ \
	args << num; \
	return args; \
}

#define defhost_signed(TYPE) \
static Lua::ReadableParams& operator>> ( \
	Lua::ReadableParams& args, \
	TYPE& num \
) \
{ \
	args.prepareReading(); \
	args.state() >> Lua::ReadableInteger<TYPE>(num); \
	args.finishReading(); \
\
	return args; \
} \
\
static Lua::ReadableParams& operator, ( \
	Lua::ReadableParams& args, \
	TYPE& num \
) \
{ \
	args >> num; \
	return args; \
} \
\
static Lua::WritableParams& operator<< ( \
	Lua::WritableParams& args, \
	TYPE num \
) \
{ \
	args.prepareWriting(); \
	args.state() << Lua::WritableInteger<TYPE>(num); \
	args.finishWriting(); \
\
	return args; \
} \
\
static Lua::WritableParams& operator, ( \
	Lua::WritableParams& args, \
	TYPE num \
) \
{ \
	args << num; \
	return args; \
}

#define defhost_unsigned(TYPE) \
static Lua::ReadableParams& operator>> ( \
	Lua::ReadableParams& args, \
	unsigned TYPE& num \
) \
{ \
	args.prepareReading(); \
	args.state() >> Lua::UnsignedReadableInteger<unsigned TYPE>(num); \
	args.finishReading(); \
\
	return args; \
} \
\
static Lua::ReadableParams& operator, ( \
	Lua::ReadableParams& args, \
	unsigned TYPE& num \
) \
{ \
	args >> num; \
	return args; \
} \
\
static Lua::WritableParams& operator<< ( \
	Lua::WritableParams& args, \
	unsigned TYPE num \
) \
{ \
	args.prepareWriting(); \
	args.state() << Lua::UnsignedWritableInteger<unsigned TYPE>(num); \
	args.finishWriting(); \
\
	return args; \
} \
\
static Lua::WritableParams& operator, ( \
	Lua::WritableParams& args, \
	unsigned TYPE num \
) \
{ \
	args << num; \
	return args; \
}
