#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include "../../../include/Closure.h"
#include "../../../include/UpValue.h"

#include "../../../include/ReadableInteger.h"
#include "../../../include/WritableInteger.h"
#include "../../../include/ReadableString.h"
#include "../../../include/WritableString.h"

BOOST_AUTO_TEST_SUITE(State__Closure)

static bool __hasBeenCalled;

static int closureFunc(lua_State* L)
{
	Lua::State state(L);

	int num = 0;
	std::string str;

	Lua::ReadableInteger<int> ri(num);
	Lua::ReadableString<std::string> rs(str);

	state >> Lua::UpValue(1) >> ri
		>> Lua::UpValue(2) >> rs;

	BOOST_TEST(num == 123);
	BOOST_TEST(str == "Hi!");

	__hasBeenCalled = true;

	return 0;
}

BOOST_AUTO_TEST_CASE(shouldBeCalled)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	Lua::WritableStackSlot slot(state);

	slot.prepare();
	{
		Lua::Closure closure(slot, closureFunc);

		closure.upValues()
			<< Lua::WritableInteger<int>(123)
			<< Lua::WritableString<std::string>("Hi!");

		closure.insert();
	}
	slot.finish();

	__hasBeenCalled = false;
	state.pcall();
	BOOST_TEST(__hasBeenCalled);

}

BOOST_AUTO_TEST_SUITE_END()
