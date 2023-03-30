#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include <LuaCpp2/LuaCpp2.h>

BOOST_AUTO_TEST_SUITE(State__CreatingAndCopying)

BOOST_AUTO_TEST_CASE(testCreateState)
{
	Lua::Library::State* L;

	{
		Lua::State state;

		BOOST_TEST(!state.isOpen());

		{
			bool exceptionOccurred = false;

			try {
				state.getL();
			}
			catch (std::exception& e) {
				exceptionOccurred = true;
				BOOST_TEST(e.what() == "invalid engine state");
			}

			BOOST_TEST(exceptionOccurred);
		}

		state.open();
		BOOST_TEST(state.isOpen());

		L = state.getL();
		BOOST_TEST(Lua::Library::inst().usecount(L) == 1);
	}

	BOOST_TEST(Lua::Library::inst().usecount(L) == 0);
}

BOOST_AUTO_TEST_CASE(testCopyState)
{
	Lua::Library::State* L;

	{
		Lua::State state;

		BOOST_TEST(!state.isOpen());
		state.open();
		BOOST_TEST(state.isOpen());

		L = state.getL();
		BOOST_TEST(Lua::Library::inst().usecount(L) == 1);

		{
			Lua::State state2{state};

			BOOST_TEST(state2.isOpen());
			BOOST_TEST(state2.getL() == L);
			BOOST_TEST(Lua::Library::inst().usecount(L) == 2);
		}

		BOOST_TEST(Lua::Library::inst().usecount(L) == 1);
	}

	BOOST_TEST(Lua::Library::inst().usecount(L) == 0);
}

BOOST_AUTO_TEST_CASE(testCopyAssignmentOperator)
{
	Lua::Library::State* L1 = nullptr;
	Lua::Library::State* L2 = nullptr;

	{
		Lua::State state1;

		BOOST_TEST(!state1.isOpen());
		state1.open();
		BOOST_TEST(state1.isOpen());

		L1 = state1.getL();
		BOOST_TEST(Lua::Library::inst().usecount(L1) == 1);

		state1.open();
		BOOST_TEST(state1.getL() == L1);
		BOOST_TEST(Lua::Library::inst().usecount(L1) == 1);

		{
			Lua::State state2;

			BOOST_TEST(!state2.isOpen());
			state2.open();
			BOOST_TEST(state2.isOpen());

			L2 = state2.getL();
			BOOST_TEST(Lua::Library::inst().usecount(L2) == 1);

			state1 = state2;

			BOOST_TEST(Lua::Library::inst().usecount(L1) == 0);
			BOOST_TEST(Lua::Library::inst().usecount(L2) == 2);
		}

		BOOST_TEST(Lua::Library::inst().usecount(L2) == 1);

	}

	BOOST_TEST(Lua::Library::inst().usecount(L2) == 0);
}

BOOST_AUTO_TEST_SUITE_END()