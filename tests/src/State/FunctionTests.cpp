#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include "../../../include/luacpp/Function.h"

BOOST_AUTO_TEST_SUITE(State__Function)

BOOST_AUTO_TEST_CASE(shouldShareMutableValue)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	int counter = 1;
	int result = 0;
	
	Lua::Function f(
		[counter, &result] (Lua::State&) mutable
		{
			result = counter++;
			return 0;
		}
	);

	state << f;
	state.pcall();
	BOOST_TEST(result == 1);

	Lua::Function f2;
	state << f;
	state >> f2;

	state << f2;
	state.pcall();
	BOOST_TEST(result == 2);

	state << f;
	state.pcall();
	BOOST_TEST(result == 3);

	state << f2;
	state.pcall();
	BOOST_TEST(result == 4);
	
}

BOOST_AUTO_TEST_CASE(shouldHaveSameState)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	Lua::Function f(
		[] (Lua::State&)
		{
			return 0;
		}
	);

	Lua::Function f2;
	state << f;
	state >> f2;

	Lua::State::SharedPtr f2StatePtr = f2.getWeakStatePtr().lock();

	BOOST_TEST(f2StatePtr);
	BOOST_TEST(state.getL() == f2StatePtr->getL());
}

BOOST_AUTO_TEST_CASE(testDestroyStateBeforeFunction)
{
	Lua::Function f;

	{
		Lua::State state;

		state.open();
		BOOST_TEST(state.isOpen());

		Lua::Function f2(
			[](Lua::State&)
			{
				return 0;
			}
		);

		state << f2;
		state >> f;
	}

}

BOOST_AUTO_TEST_SUITE_END()