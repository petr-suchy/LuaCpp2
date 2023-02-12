#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include "../../../include/BasicHostDefs.h"

#include "ArgumentCounter.h"
#include "WrappedPointerTo.h"

struct Point {
	int x;
	int y;
};

struct StaticPoint {
	const int x = 123;
	const int y = 456;
};

struct Rectangle {
	Point leftTop;
	Point rightBottom;
};

defhost_struct(Point, m.x, m.y)
defhost_struct(Rectangle, m.leftTop, m.rightBottom)

defhost_const_struct(StaticPoint, m.x, m.y)

BOOST_AUTO_TEST_SUITE(Parameters__Parameters)

BOOST_AUTO_TEST_CASE(testBoolean)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		WritableArgumentCounter args(state);

		args << false << true;

		BOOST_TEST(args.count() == 2);
		BOOST_TEST(state.getStackTop() == 2);
	}

	{
		ReadableArgumentCounter args(state);

		bool b1 = false;
		bool b2 = true;

		args >> b1 >> b2;

		BOOST_TEST(args.count() == 2);
		BOOST_TEST(state.getStackTop() == 0);

		BOOST_TEST(b1 == true);
		BOOST_TEST(b2 == false);
	}

}

BOOST_AUTO_TEST_CASE(testNumber)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		WritableArgumentCounter args(state);

		float x = 1.5;
		double y = 123456.789;

		args << x << y;

		BOOST_TEST(args.count() == 2);
		BOOST_TEST(state.getStackTop() == 2);
	}

	{
		ReadableArgumentCounter args(state);

		float x;
		double y;

		args >> y >> x;

		BOOST_TEST(args.count() == 2);
		BOOST_TEST(state.getStackTop() == 0);

		BOOST_TEST(x == 1.5);
		BOOST_TEST(y == 123456.789);
	}

}

BOOST_AUTO_TEST_CASE(testInteger)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		WritableArgumentCounter args(state);

		int x = 10;
		int y = 20;

		args << x << y;

		BOOST_TEST(args.count() == 2);
		BOOST_TEST(state.getStackTop() == 2);
	}

	{
		ReadableArgumentCounter args(state);

		int x, y;

		args >> y >> x;

		BOOST_TEST(args.count() == 2);
		BOOST_TEST(state.getStackTop() == 0);

		BOOST_TEST(x == 10);
		BOOST_TEST(y == 20);
	}

}

BOOST_AUTO_TEST_CASE(testString)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		WritableArgumentCounter args(state);

		const std::string str1 = "foo";
		const std::string str2 = "bar";

		args << str1 << str2;

		BOOST_TEST(args.count() == 2);
		BOOST_TEST(state.getStackTop() == 2);
	}

	{
		ReadableArgumentCounter args(state);

		std::string str1;
		std::string str2;

		args >> str2 >> str1;

		BOOST_TEST(args.count() == 2);
		BOOST_TEST(state.getStackTop() == 0);

		BOOST_TEST(str1 == "foo");
		BOOST_TEST(str2 == "bar");
	}

}

BOOST_AUTO_TEST_CASE(testPointer)
{
	int dtorCounter = 0;

	{
		Lua::State state;

		state.open();
		BOOST_TEST(state.isOpen());
		
		{
			auto wrappedPtr = std::make_shared<
				WrappedPointerTo<int>
			>([&dtorCounter]() { dtorCounter++; }, 123456);

			Lua::Pointer<
				std::shared_ptr<
					WrappedPointerTo<int>
				>
			> ptr(wrappedPtr);

			WritableArgumentCounter args(state);

			args << ptr;

			BOOST_TEST(args.count() == 1);
			BOOST_TEST(state.getStackTop() == 1);
		}

		BOOST_TEST(dtorCounter == 0);
		
		{
			Lua::Pointer<
				std::shared_ptr<
					WrappedPointerTo<int>
				>
			> ptr;

			ReadableArgumentCounter args(state);
				
			args >> ptr;

			BOOST_TEST(args.count() == 1);
			BOOST_TEST(state.getStackTop() == 0);

			BOOST_TEST(ptr.get()->getTestValue() == 123456);
		}

		BOOST_TEST(dtorCounter == 0);
	
	}

	BOOST_TEST(dtorCounter == 1);

}

BOOST_AUTO_TEST_CASE(testFunction)
{
	int callCounter = 0;
	int callCounterResult = 0;

	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		WritableArgumentCounter args(state);

		Lua::Function func(
			[callCounter, &callCounterResult] (Lua::State&) mutable
			{
				callCounterResult = ++callCounter;
				return 0;
			}
		);

		args << func;
 
		BOOST_TEST(args.count() == 1);
		BOOST_TEST(state.getStackTop() == 1);
	}
	
	{
		Lua::Function func;

		ReadableArgumentCounter args(state);

		args >> func;

		BOOST_TEST(args.count() == 1);
		BOOST_TEST(state.getStackTop() == 0);

		state << func;
		state.pcall();
		BOOST_TEST(callCounterResult == 1);

		state << func;
		state.pcall();
		BOOST_TEST(callCounterResult == 2);

		state << func;
		state.pcall();
		BOOST_TEST(callCounterResult == 3);
	}
	
}

BOOST_AUTO_TEST_CASE(testStruct)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		WritableArgumentCounter args(state);

		Point point = { 10, 20 };
		args << point;

		BOOST_TEST(args.count() == 1);
		BOOST_TEST(state.getStackTop() == 1);
	}

	{
		ReadableArgumentCounter args(state);

		Point point;
		args >> point;

		BOOST_TEST(args.count() == 1);
		BOOST_TEST(state.getStackTop() == 0);

		BOOST_TEST(point.x == 10);
		BOOST_TEST(point.y == 20);
	}

}

BOOST_AUTO_TEST_CASE(testNestedStruct)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		WritableArgumentCounter args(state);

		Rectangle rect = {
			{ 10, 20 },
			{ 100, 200 }
		};

		args << rect;

		BOOST_TEST(args.count() == 1);
		BOOST_TEST(state.getStackTop() == 1);
	}

	{
		ReadableArgumentCounter args(state);

		Rectangle rect;
		args >> rect;

		BOOST_TEST(args.count() == 1);
		BOOST_TEST(state.getStackTop() == 0);

		BOOST_TEST(rect.leftTop.x == 10);
		BOOST_TEST(rect.leftTop.y == 20);
		BOOST_TEST(rect.rightBottom.x == 100);
		BOOST_TEST(rect.rightBottom.y == 200);
	}

}

BOOST_AUTO_TEST_CASE(testConstStruct)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		WritableArgumentCounter args(state);

		StaticPoint point;
		args << point;

		BOOST_TEST(args.count() == 1);
		BOOST_TEST(state.getStackTop() == 1);
	}

	{
		ReadableArgumentCounter args(state);

		StaticPoint point;
		args >> point;

		BOOST_TEST(args.count() == 1);
		BOOST_TEST(state.getStackTop() == 0);
	}

}

BOOST_AUTO_TEST_CASE(testConstStruct2)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		WritableArgumentCounter args(state);

		StaticPoint point;
		args << point;

		BOOST_TEST(args.count() == 1);
		BOOST_TEST(state.getStackTop() == 1);
	}

	{
		ReadableArgumentCounter args(state);

		Point point;
		args >> point;

		BOOST_TEST(args.count() == 1);
		BOOST_TEST(state.getStackTop() == 0);

		BOOST_TEST(point.x == 123);
		BOOST_TEST(point.y == 456);
	}

}

BOOST_AUTO_TEST_CASE(testIgnore)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		WritableArgumentCounter args(state);
		args << 10 << 20 << 30;
	}

	{
		ReadableArgumentCounter args(state);
		int n = 0;

		args >> Lua::IgnoreParam{} >> n >> Lua::IgnoreParam{};

		BOOST_TEST(state.getStackTop() == 0);
		BOOST_TEST(n == 20);
	}

}

BOOST_AUTO_TEST_SUITE_END()