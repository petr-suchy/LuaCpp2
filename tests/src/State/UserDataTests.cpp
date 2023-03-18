#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include <LuaCpp2/UserData.h>

BOOST_AUTO_TEST_SUITE(State__UserData)

class WrappedType {
public:

	typedef std::function<void()> DtorFunc;

	WrappedType(DtorFunc dtorFunc, int testValue = 0) :
		_dtorFunc(dtorFunc),
		_testValue(testValue)
	{}

	~WrappedType()
	{
		_dtorFunc();
	}

	int getTestValue()
	{
		return _testValue;
	}

private:

	DtorFunc _dtorFunc;
	int _testValue;

};

BOOST_AUTO_TEST_CASE(shouldBeDestroyedInHostScope)
{
	int dtorCounter = 0;

	WrappedType* wrappedTypePtr = new WrappedType(
		[&dtorCounter] ()
		{
			dtorCounter++;
		}
	);

	{
		Lua::UserData<WrappedType> userData(wrappedTypePtr);
	}

	BOOST_TEST(dtorCounter == 1);
}

BOOST_AUTO_TEST_CASE(shouldBeDestroyedInEngineScope)
{
	int dtorCounter = 0;

	WrappedType* wrappedTypePtr = new WrappedType(
		[&dtorCounter] ()
		{
			dtorCounter++;
		}
	);

	{
		Lua::State state;
		state.open();
		BOOST_TEST(state.isOpen());

		{
			Lua::UserData<WrappedType> userData(wrappedTypePtr);
			state << userData;
		}

		BOOST_TEST(dtorCounter == 0);
	}

	BOOST_TEST(dtorCounter == 1);
}

BOOST_AUTO_TEST_CASE(shouldBeInsertedToStack)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	Lua::UserData<int> userData(new int(123456));

	state << userData;

	BOOST_TEST(state.getStackTop() == 1);
	BOOST_TEST(state.isValueAt(Lua::State::StackTop, Lua::Library::Type::Userdata));
}

BOOST_AUTO_TEST_CASE(shouldBeGotFromStack)
{
	int dtorCounter = 0;
	int testValue = 123456;

	WrappedType* wrappedTypePtr = new WrappedType(
		[&dtorCounter] ()
		{
			dtorCounter++;
		},
		testValue
	);

	WrappedType* wrappedTypePtr2 = nullptr;

	{
		Lua::State state;

		state.open();
		BOOST_TEST(state.isOpen());

		{
			Lua::UserData<WrappedType> userData(wrappedTypePtr);
			state << userData;
		}

		BOOST_TEST(dtorCounter == 0);

		{
			Lua::UserData<WrappedType> userData2;
			state >> userData2;
			wrappedTypePtr2 = userData2.ptr();
		}

		BOOST_TEST(dtorCounter == 0);

		BOOST_TEST(wrappedTypePtr == wrappedTypePtr2);
		BOOST_TEST(wrappedTypePtr2->getTestValue() == testValue);
	}

	BOOST_TEST(dtorCounter == 1);

}

BOOST_AUTO_TEST_SUITE_END()


