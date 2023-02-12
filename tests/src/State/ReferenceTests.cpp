#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include "../../../include/Reference.h"
#include "../../../include/ReadableInteger.h"
#include "../../../include/WritableInteger.h"

BOOST_AUTO_TEST_SUITE(State__Reference)

BOOST_AUTO_TEST_CASE(testInsertionAndRetrieval)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	state << Lua::WritableInteger<int>(123);

	Lua::Reference ref;
	state >> ref;

	BOOST_TEST(state.getStackTop() == 0);

	state << ref;

	BOOST_TEST(state.getStackTop() == 1);

	int num;
	Lua::ReadableInteger<int> ri(num);
	state >> ri;

	BOOST_TEST(num == 123);
}

BOOST_AUTO_TEST_SUITE_END()
