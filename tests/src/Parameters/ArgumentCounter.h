#pragma once

#include <LuaCpp2/ReadableParams.h>
#include <LuaCpp2/WritableParams.h>

class ReadableArgumentCounter : public Lua::ReadableParams {
public:

	ReadableArgumentCounter(Lua::State& state) :
		_numOfReadArguments(0),
		ReadableParams(
			state,
			// gets count
			[this] (Lua::State&)
			{
				return _numOfReadArguments;
			},
			// prepares reading
			[] (Lua::State&)
			{},
			// finishes reading
			[this] (Lua::State&) mutable
			{
				_numOfReadArguments++;
			}
		)
	{}

private:
	int _numOfReadArguments;
};

class WritableArgumentCounter : public Lua::WritableParams {
public:

	WritableArgumentCounter(Lua::State& state) :
		_numOfWrittenArguments(0),
		WritableParams(
			state,
			// gets count
			[this] (Lua::State&)
			{
				return _numOfWrittenArguments;
			},
			// prepares writing
			[] (Lua::State&)
			{},
			// finishes writing
			[this] (Lua::State&) mutable
			{
				_numOfWrittenArguments++;
			}
		)
	{}

private:
	int _numOfWrittenArguments;
};
