#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include "../../../include/ReadableStackSlot.h"
#include "../../..//include/WritableStackSlot.h"

BOOST_AUTO_TEST_SUITE(State__StackSlot)

BOOST_AUTO_TEST_CASE(testUnfinishedWriting)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		Lua::WritableStackSlot slot(state);

		slot.prepare();
		slot.insertNumber(123);
		// slot.finish();
	}

	// The stack item should be discarded at the end of the block.
	BOOST_TEST(state.getStackTop() == 0);
}

BOOST_AUTO_TEST_CASE(testInsertionAndRetrieval)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		Lua::WritableStackSlot slot(state);

		slot.prepare();
		slot.insertInteger(123);
		slot.finish();
	}

	// the item should be kept at the stack
	BOOST_TEST(state.getStackTop() == 1);
	
	int num = 0;

	{
		Lua::ReadableStackSlot slot(state);

		slot.prepare();
		num = (int) slot.getInteger();
		slot.finish();
	}

	BOOST_TEST(num == 123);
	BOOST_TEST(state.getStackTop() == 0);
}

BOOST_AUTO_TEST_SUITE_END()