#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include <LuaCpp2/ReadableChunk.h>
#include <LuaCpp2/WritableChunk.h>
#include <LuaCpp2/ReadableInteger.h>
#include <LuaCpp2/WritableInteger.h>

#include <sstream>

BOOST_AUTO_TEST_SUITE(State__Chunk)

BOOST_AUTO_TEST_CASE(testInsertionAndRetrieval)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		std::istringstream is(
			"return 123"
		);

		state << Lua::WritableChunk<std::istream>(is);

		BOOST_TEST(state.getStackTop() == 1);
		BOOST_TEST(state.isValueAt(Lua::State::StackTop, Lua::Library::Type::Function));
	}

	// the chunk has been left on the stack
	
	{
		std::stringstream ss;
		Lua::ReadableChunk<std::ostream> rc(ss);

		state >> rc;
		BOOST_TEST(ss.str().length() != 0);

		state << Lua::WritableChunk<std::istream>(ss);

		BOOST_TEST(state.getStackTop() == 1);
		BOOST_TEST(state.isValueAt(Lua::State::StackTop, Lua::Library::Type::Function));

		state.pcall();
		BOOST_TEST(state.getStackTop() == 1);

		int result = 0;
		Lua::ReadableInteger<int> ri(result);
		state >> ri;

		BOOST_TEST(state.getStackTop() == 0);
		BOOST_TEST(result == 123);
	}
	
}

BOOST_AUTO_TEST_SUITE_END()
