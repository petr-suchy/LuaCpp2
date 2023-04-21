#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include <LuaCpp2/LuaCpp2.h>

struct ShortPoint {
	char x;
	char y;
};

struct LongPoint {
	int x;
	int y;
};

struct SmallRectangle {
	ShortPoint leftTop;
	ShortPoint rightBottom;
};

struct BigRectangle {
	LongPoint leftTop;
	LongPoint rightBottom;
};

class ErrorValue : public Lua::ReadableValue, public Lua::WritableValue {
public:

	virtual void getFrom(Lua::State& state)
	{
		throw std::logic_error("test exception");
	}

	virtual void insertTo(Lua::State& state) const
	{
		throw std::logic_error("test exception");
	}

};

static Lua::WritableParams operator<< (
	Lua::WritableParams args,
	const ErrorValue& val
)
{
	args.prepareWriting();
	args.state() << ErrorValue();
	args.finishWriting();

	return args;
}

struct TableWithError {
	ErrorValue val;
};

struct NestedTableWithError {
	TableWithError tbl;
};

struct TableWithFunction {
	Lua::Function func;
};

defhost_struct(ShortPoint, m.x, m.y)
defhost_struct(LongPoint, m.x, m.y)
defhost_struct(SmallRectangle, m.leftTop, m.rightBottom)
defhost_struct(BigRectangle, m.leftTop, m.rightBottom)

defhost_struct(TableWithError, m.val)
defhost_struct(NestedTableWithError, m.tbl)
defhost_struct(TableWithFunction, m.func)

BOOST_AUTO_TEST_SUITE(Engine__Engine)

BOOST_AUTO_TEST_CASE(testStackIntegrity1)
{
	Lua::Engine lua;

	{
		bool exceptionOccurred = false;

		try {

			NestedTableWithError nestedTblWithError;

			lua.global("tbl").in() << nestedTblWithError;

		}
		catch (...) {
			exceptionOccurred = true;
		}

		BOOST_TEST(exceptionOccurred);
	}

	{
		BigRectangle rect = {
			{ 1, 2 },
			{ 10, 200 }
		};

		lua.global("rect").in() << rect;
	}

}

BOOST_AUTO_TEST_CASE(testStackIntegrity2)
{
	Lua::Engine lua;

	{
		BigRectangle rect = {
			{ 1, 2 },
			{ 10, 200 }
		};

		lua.global("rect").in() << rect;
	}

	{
		bool exceptionOccurred = false;

		try {

			SmallRectangle rect;
			lua.global("rect").out() >> rect;

		}
		catch (...) {
			exceptionOccurred = true;
		}

		BOOST_TEST(exceptionOccurred);
	}

	{
		BigRectangle rect;
		lua.global("rect").out() >> rect;

		BOOST_TEST(rect.leftTop.x == 1);
		BOOST_TEST(rect.leftTop.y == 2);
		BOOST_TEST(rect.rightBottom.x == 10);
		BOOST_TEST(rect.rightBottom.y == 200);
	}

}

BOOST_AUTO_TEST_CASE(testException)
{
	Lua::Engine lua;

	auto func = Lua::MakeFunc(
		[] (Lua::Args args, Lua::Lua lua)
		{
			int x, y;
			args.in() >> x >> y;
			args.out() << (x + y);
		}
	);

	{
		int result = 0;
		bool exceptionOccurred = false;

		try {
			lua.pcall(func, 10) >> result;
		}
		catch (...) {
			exceptionOccurred = true;
		}
		
		BOOST_TEST(exceptionOccurred);
		BOOST_TEST(lua.state().getStackTop() == 0);
		BOOST_TEST(lua.args().in().count() == 0);
		BOOST_TEST(lua.args().out().count() == 0);
	}
	
	{
		int result = 0;
		bool exceptionOccurred = false;

		try {
			lua.pcall(func, 10, 20) >> result;
		}
		catch (...) {
			exceptionOccurred = true;
		}

		BOOST_TEST(!exceptionOccurred);
		BOOST_TEST(result == 30);
	}
	
}

BOOST_AUTO_TEST_CASE(testTransfer)
{
	Lua::Engine local;
	Lua::Engine remote;

	BigRectangle rect1 = {
		{ 1, 2 },
		{ 100, 200 }
	};

	local.global("rect1").in() << rect1;

	remote.global("rect2").in() << local.global("rect1").out();

	BigRectangle rect2 = { 0 };

	remote.global("rect2").out() >> rect2;

	BOOST_TEST(rect1.leftTop.x == rect2.leftTop.x);
	BOOST_TEST(rect1.leftTop.y == rect2.leftTop.y);
	BOOST_TEST(rect1.rightBottom.x == rect2.rightBottom.x);
	BOOST_TEST(rect1.rightBottom.y == rect2.rightBottom.y);
}

BOOST_AUTO_TEST_CASE(testInitFunc)
{
	Lua::Engine lua(
		[](Lua::Lua lua)
		{
			lua.global("foo").in() << 123;
		}
	);

	int foo;
	lua.global("foo").out() >> foo;

	BOOST_TEST(foo == 123);
}

BOOST_AUTO_TEST_CASE(testFunctionInsideTable)
{
	Lua::Engine lua(
		[](Lua::Lua lua)
		{
			lua.openStdLibs();

			lua.global("test").in() << Lua::MakeFunc(
				[](Lua::Args args, Lua::Lua lua)
				{
					TableWithFunction tbl;
					args.in() >> tbl;
					args.out() << tbl;
				}
			);

		}
	);

	lua.doString(
		"tbl = {func = function(a, b) return a + b end}\n"
		"tbl2 = test(tbl)"
	);

	TableWithFunction tbl2;
	lua.global("tbl2").out() >> tbl2;

	int result;
	lua.pcall(tbl2.func, 10, 20) >> result;

	BOOST_TEST(result == 30);
}

BOOST_AUTO_TEST_CASE(testErrorString)
{
	Lua::Engine lua(
		[](Lua::Lua lua)
		{
			lua.openStdLibs();

			lua.global("functionWithError").in() << Lua::MakeFunc(
				[](Lua::Args args, Lua::Lua lua)
				{
					throw std::logic_error("test error string");
				}
			);

		}
	);

	std::string errorString;

	try {
		lua.pcall("functionWithError");
	}
	catch (std::exception& e) {
		errorString = e.what();
	}

	BOOST_TEST(errorString == "test error string");
}

BOOST_AUTO_TEST_CASE(testIsGlobal)
{
	Lua::Engine lua;

	BOOST_TEST(!lua.isGlobal("foo"));
	lua.global("foo").in() << 123;
	BOOST_TEST(lua.isGlobal("foo"));
	BOOST_TEST(lua.state().getStackTop() == 0);
}

BOOST_AUTO_TEST_SUITE_END()