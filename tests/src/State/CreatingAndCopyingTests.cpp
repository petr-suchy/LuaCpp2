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

BOOST_AUTO_TEST_SUITE_END()