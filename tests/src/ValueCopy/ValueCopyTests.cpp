#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include "../../../include/luacpp/Reference.h"
#include "../../../include/luacpp/ReadableInteger.h"
#include "../../../include/luacpp/WritableInteger.h"
#include "../../../include/luacpp/ValueCopy.h"

BOOST_AUTO_TEST_SUITE(ValueCopy__ValueCopy)

BOOST_AUTO_TEST_CASE(testInsertionAndRetrieval)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	state << Lua::WritableInteger<int>(123);

	Lua::ValueCopy copy;
	state >> copy;

	BOOST_TEST(state.getStackTop() == 0);

	state << copy;

	BOOST_TEST(state.getStackTop() == 1);

	int num;
	Lua::ReadableInteger<int> rn(num);
	state >> rn;

	BOOST_TEST(num == 123);
}

BOOST_AUTO_TEST_SUITE_END()
