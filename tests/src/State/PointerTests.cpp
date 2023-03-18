#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include <LuaCpp2/Pointer.h>

BOOST_AUTO_TEST_SUITE(State__Pointer)

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

	{
		std::shared_ptr<WrappedType> wrappedPtr = std::make_shared<WrappedType>(
			[&dtorCounter] ()
			{
				dtorCounter++;
			}
		);
		
		Lua::Pointer<
			std::shared_ptr<WrappedType>
		> ptr(wrappedPtr);
		
	}

	BOOST_TEST(dtorCounter == 1);
}

BOOST_AUTO_TEST_CASE(shouldBeDestroyedInHostScope2)
{
	int dtorCounter = 0;

	{
		std::shared_ptr<WrappedType> wrappedPtr = std::make_shared<WrappedType>(
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
				Lua::Pointer<
					std::shared_ptr<WrappedType>
				> ptr(wrappedPtr);

				state << ptr;
			}

			BOOST_TEST(dtorCounter == 0);
		}

		BOOST_TEST(dtorCounter == 0);

	}

	BOOST_TEST(dtorCounter == 1);
}

BOOST_AUTO_TEST_CASE(shouldBeDestroyedInEngineScope)
{
	int dtorCounter = 0;

	{
		Lua::State state;

		state.open();
		BOOST_TEST(state.isOpen());

		{
			std::shared_ptr<WrappedType> wrappedPtr = std::make_shared<WrappedType>(
				[&dtorCounter]()
				{
					dtorCounter++;
				}
			);

			Lua::Pointer<
				std::shared_ptr<WrappedType>
			> ptr(wrappedPtr);

			state << ptr;
		}

		BOOST_TEST(dtorCounter == 0);

	}

	BOOST_TEST(dtorCounter == 1);
}

BOOST_AUTO_TEST_CASE(shouldBeInsertedToStack)
{
	int dtorCounter = 0;

	{
		Lua::State state;

		state.open();
		BOOST_TEST(state.isOpen());

		std::shared_ptr<WrappedType> wrappedPtr = std::make_shared<WrappedType>(
			[&dtorCounter]()
			{
				dtorCounter++;
			}
		);

		{
			Lua::Pointer<
				std::shared_ptr<WrappedType>
			> ptr(wrappedPtr);

			state << ptr;
		}

		BOOST_TEST(dtorCounter == 0);

		BOOST_TEST(state.getStackTop() == 1);
		BOOST_TEST(state.isValueAt(Lua::State::StackTop, Lua::Library::Type::Userdata));
	}

	BOOST_TEST(dtorCounter == 1);
}

BOOST_AUTO_TEST_CASE(shouldBeGotFromStack)
{
	int dtorCounter = 0;
	int testValue = 123456;

	{
		Lua::State state;

		state.open();
		BOOST_TEST(state.isOpen());

		std::shared_ptr<WrappedType> wrappedPtr = std::make_shared<WrappedType>(
			[&dtorCounter]()
			{
				dtorCounter++;
			},
			testValue
		);

		std::shared_ptr<WrappedType> wrappedPtr2 = nullptr;

		{
			Lua::Pointer<
				std::shared_ptr<WrappedType>
			> ptr(wrappedPtr);

			state << ptr;
		}

		BOOST_TEST(dtorCounter == 0);

		{
			Lua::Pointer<
				std::shared_ptr<WrappedType>
			> ptr2;

			state >> ptr2;
			wrappedPtr2 = ptr2.get();
		}

		BOOST_TEST(dtorCounter == 0);

		BOOST_TEST(wrappedPtr.get() == wrappedPtr2.get());
		BOOST_TEST(wrappedPtr2->getTestValue() == testValue);
	}

	BOOST_TEST(dtorCounter == 1);
}

BOOST_AUTO_TEST_SUITE_END()
