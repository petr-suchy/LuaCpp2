#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include <LuaCpp2/LuaCpp2.h>

BOOST_AUTO_TEST_SUITE(Engine__FunctionCall)

BOOST_AUTO_TEST_CASE(testUnreadInputArguments)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			if (args.in().count() != 0) {
				throw std::logic_error(
					"unexpected input arguments"
				);
			}
		}
	);

	lua.global("bar").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			if (args.in().count() != 2) {
				throw std::logic_error(
					"unexpected number of input arguments"
				);
			}

			lua.pcall("foo");

			if (args.out().count() != 0) {
				throw std::logic_error(
					"unexpected output arguments"
				);
			}
		}
	);

	lua.pcall("bar", 1, 2);
}

BOOST_AUTO_TEST_CASE(testUnreadInputArguments2)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			if (args.in().count() != 2) {
				throw std::logic_error(
					"unexpected number of input arguments"
				);
			}

			int n1;
			args.in() >> n1;

			if (n1 != 3) {
				throw std::logic_error(
					"unexpected value of the first input argument (" + std::to_string(n1) + ")"
				);
			}

			int n2;
			args.in() >> n2;

			if (n2 != 4) {
				throw std::logic_error(
					"unexpected value of the second input argument (" + std::to_string(n2) + ")"
				);
			}
		}
	);

	lua.global("bar").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			if (args.in().count() != 2) {
				throw std::logic_error(
					"unexpected number of input arguments"
				);
			}

			int n1;
			args.in() >> n1;

			lua.pcall("foo", 3, 4);

			if (args.out().count() != 0) {
				throw std::logic_error(
					"unexpected output arguments"
				);
			}
		}
	);

	lua.pcall("bar", 1, 2);
}

BOOST_AUTO_TEST_CASE(testUnreadInputArguments3)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
		}
	);

	int result = 0;
	lua.pcall("foo", 1);
	BOOST_TEST(lua.args().out().count() == 0);
}

BOOST_AUTO_TEST_CASE(testUnreadInputArguments4)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			args.out() << 2;
		}
	);

	int result = 0;
	lua.pcall("foo", 1) >> result;
	BOOST_TEST(result == 2);
}

BOOST_AUTO_TEST_CASE(testUnreadOutputArguments)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			args.out() << 1 << 2;
		}
	);

	lua.global("bar").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			if (args.in().count() != 0) {
				throw std::logic_error(
					"unexpected input arguments"
				);
			}
		}
	);

	lua.pcall("foo");
	BOOST_TEST(lua.args().out().count() == 2);
	lua.pcall("bar");
	BOOST_TEST(lua.args().out().count() == 0);
}

BOOST_AUTO_TEST_CASE(testUnreadOutputArguments2)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			args.out() << 1 << 2;
		}
	);

	lua.global("bar").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			if (args.in().count() != 2) {
				throw std::logic_error(
					"unexpected number of input arguments"
				);
			}

			int n1;
			args.in() >> n1;

			if (n1 != 3) {
				throw std::logic_error(
					"unexpected value of the first input argument (" + std::to_string(n1) + ")"
				);
			}

			int n2;
			args.in() >> n2;

			if (n2 != 4) {
				throw std::logic_error(
					"unexpected value of the second input argument (" + std::to_string(n2) + ")"
				);
			}
		}
	);

	lua.pcall("foo");
	BOOST_TEST(lua.args().out().count() == 2);

	int n1;
	lua.args().out() >> n1;
	BOOST_TEST(n1 == 1);
	BOOST_TEST(lua.args().out().count() == 1);

	lua.pcall("bar", 3, 4);
	BOOST_TEST(lua.args().out().count() == 0);
}

BOOST_AUTO_TEST_CASE(testUnreadOutputArguments3)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			args.out() << 1 << 2;
		}
	);

	lua.global("bar").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			if (args.in().count() != 2) {
				throw std::logic_error(
					"unexpected number of input arguments"
				);
			}

			int n1;
			args.in() >> n1;

			if (n1 != 3) {
				throw std::logic_error(
					"unexpected value of the first input argument (" + std::to_string(n1) + ")"
				);
			}

			int n2;
			args.in() >> n2;

			if (n2 != 4) {
				throw std::logic_error(
					"unexpected value of the second input argument (" + std::to_string(n2) + ")"
				);
			}
		}
	);

	lua.global("cow").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			int n1;
			lua.pcall("foo") >> n1;
			lua.pcall("bar", 3, 4);
		}
	);

	lua.pcall("cow");
}

BOOST_AUTO_TEST_CASE(testUnreadOutputArguments4)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			args.out() << 1;
		}
	);

	lua.global("bar").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			lua.pcall("foo");
		}
	);

	int result = 0;
	lua.pcall("bar");
	BOOST_TEST(lua.args().out().count() == 0);
}

BOOST_AUTO_TEST_CASE(testUnreadOutputArguments5)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			args.out() << 1;
		}
	);

	lua.global("bar").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			lua.pcall("foo");
			args.out() << 2;
		}
	);

	int result = 0;
	lua.pcall("bar") >> result;
	BOOST_TEST(result == 2);
}

BOOST_AUTO_TEST_CASE(testArgumentsOrder)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			int result = 0;
			int i = 1;

			while (!args.in().atEnd()) {
				int n;
				args.in() >> n;
				result += n * i;
				i *= 10;
			}

			args.out() << result;

			do {
				args.out() << result % 10;
				result /= 10;
			} while (result != 0);
		}
	);

	int result;
	int n1, n2, n3, n4, n5, n6;

	lua.args().in() << 1;
	lua.pcall("foo");
	lua.args().out() >> result >> n1;
	BOOST_TEST(result == 1);
	BOOST_TEST(n1 == 1);

	lua.args().in() << 1;
	lua.args().in() << 2;
	lua.pcall("foo") >> result >> n1 >> n2;
	BOOST_TEST(result == 21);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);

	lua.args().in() << 1;
	lua.args().in() << 2;
	lua.args().in() << 3;
	lua.pcall("foo") >> result >> n1 >> n2 >> n3;
	BOOST_TEST(result == 321);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	BOOST_TEST(n3 == 3);

	lua.args().in() << 1;
	lua.args().in() << 2;
	lua.args().in() << 3;
	lua.pcall("foo", 4) >> result >> n1 >> n2 >> n3 >> n4;
	BOOST_TEST(result == 4321);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	BOOST_TEST(n3 == 3);
	BOOST_TEST(n4 == 4);

	lua.args().in() << 1;
	lua.args().in() << 2;
	lua.args().in() << 3;
	lua.pcall("foo", 4, 5) >> result >> n1 >> n2 >> n3 >> n4 >> n5;
	BOOST_TEST(result == 54321);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	BOOST_TEST(n3 == 3);
	BOOST_TEST(n4 == 4);
	BOOST_TEST(n5 == 5);

	lua.pcall("foo", 1, 2, 3, 4, 5, 6) >> result >> n1 >> n2 >> n3 >> n4 >> n5 >> n6;
	BOOST_TEST(result == 654321);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	BOOST_TEST(n3 == 3);
	BOOST_TEST(n4 == 4);
	BOOST_TEST(n5 == 5);
	BOOST_TEST(n6 == 6);
}

BOOST_AUTO_TEST_CASE(testForwardArguments)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			int result = 0;
			int i = 1;

			while (!args.in().atEnd()) {
				int n;
				args.in() >> n;
				result += n * i;
				i *= 10;
			}

			args.out() << result;

			do {
				args.out() << result % 10;
				result /= 10;
			} while (result != 0);
		}
	);

	lua.global("bar").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			while (!args.in().atEnd()) {
				lua.args().in() << args.in();
			}

			lua.pcall("foo");

			while (!lua.args().out().atEnd()) {
				lua.args().out() >> args.out();
			}
		}
	);

	int result;
	int n1, n2, n3, n4, n5, n6;

	lua.args().in() << 1;
	lua.pcall("bar");
	lua.args().out() >> result >> n1;
	BOOST_TEST(result == 1);
	BOOST_TEST(n1 == 1);
	
	lua.args().in() << 1;
	lua.args().in() << 2;
	lua.pcall("bar") >> result >> n1 >> n2;
	BOOST_TEST(result == 21);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	
	lua.args().in() << 1;
	lua.args().in() << 2;
	lua.args().in() << 3;
	lua.pcall("bar") >> result >> n1 >> n2 >> n3;
	BOOST_TEST(result == 321);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	BOOST_TEST(n3 == 3);

	lua.args().in() << 1;
	lua.args().in() << 2;
	lua.args().in() << 3;
	lua.pcall("bar", 4) >> result >> n1 >> n2 >> n3 >> n4;
	BOOST_TEST(result == 4321);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	BOOST_TEST(n3 == 3);
	BOOST_TEST(n4 == 4);

	lua.args().in() << 1;
	lua.args().in() << 2;
	lua.args().in() << 3;
	lua.pcall("bar", 4, 5) >> result >> n1 >> n2 >> n3 >> n4 >> n5;
	BOOST_TEST(result == 54321);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	BOOST_TEST(n3 == 3);
	BOOST_TEST(n4 == 4);
	BOOST_TEST(n5 == 5);

	lua.pcall("bar", 1, 2, 3, 4, 5, 6) >> result >> n1 >> n2 >> n3 >> n4 >> n5 >> n6;
	BOOST_TEST(result == 654321);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	BOOST_TEST(n3 == 3);
	BOOST_TEST(n4 == 4);
	BOOST_TEST(n5 == 5);
	BOOST_TEST(n6 == 6);
}

BOOST_AUTO_TEST_CASE(testForwardArguments2)
{
	Lua::Engine lua1;
	Lua::Engine lua2;

	lua1.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua1)
		{
			int result = 0;
			int i = 1;

			while (!args.in().atEnd()) {
				int n;
				args.in() >> n;
				result += n * i;
				i *= 10;
			}

			args.out() << result;

			do {
				args.out() << result % 10;
				result /= 10;
			} while (result != 0);
		}
	);

	lua2.global("bar").in() << Lua::MakeFunc(
		[&lua1](Lua::Args args, Lua::Lua lua2)
		{
			while (!args.in().atEnd()) {
				args.in() >> lua1.args().in();
			}

			lua1.pcall("foo");

			while (!lua1.args().out().atEnd()) {
				args.out() << lua1.args().out();
			}
		}
	);

	int result;
	int n1, n2, n3, n4, n5, n6;

	lua2.args().in() << 1;
	lua2.pcall("bar");
	lua2.args().out() >> result >> n1;
	BOOST_TEST(result == 1);
	BOOST_TEST(n1 == 1);

	lua2.args().in() << 1;
	lua2.args().in() << 2;
	lua2.pcall("bar") >> result >> n1 >> n2;
	BOOST_TEST(result == 21);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);

	lua2.args().in() << 1;
	lua2.args().in() << 2;
	lua2.args().in() << 3;
	lua2.pcall("bar") >> result >> n1 >> n2 >> n3;
	BOOST_TEST(result == 321);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	BOOST_TEST(n3 == 3);

	lua2.args().in() << 1;
	lua2.args().in() << 2;
	lua2.args().in() << 3;
	lua2.pcall("bar", 4) >> result >> n1 >> n2 >> n3 >> n4;
	BOOST_TEST(result == 4321);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	BOOST_TEST(n3 == 3);
	BOOST_TEST(n4 == 4);

	lua2.args().in() << 1;
	lua2.args().in() << 2;
	lua2.args().in() << 3;
	lua2.pcall("bar", 4, 5) >> result >> n1 >> n2 >> n3 >> n4 >> n5;
	BOOST_TEST(result == 54321);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	BOOST_TEST(n3 == 3);
	BOOST_TEST(n4 == 4);
	BOOST_TEST(n5 == 5);

	lua2.pcall("bar", 1, 2, 3, 4, 5, 6) >> result >> n1 >> n2 >> n3 >> n4 >> n5 >> n6;
	BOOST_TEST(result == 654321);
	BOOST_TEST(n1 == 1);
	BOOST_TEST(n2 == 2);
	BOOST_TEST(n3 == 3);
	BOOST_TEST(n4 == 4);
	BOOST_TEST(n5 == 5);
	BOOST_TEST(n6 == 6);
}

BOOST_AUTO_TEST_CASE(testCallFunctionVar)
{
	Lua::Engine lua;

	Lua::Function foo = Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			int x, y;
			args.in() >> x >> y;
			args.out() << x + y;
		}
	);

	int result;
	lua.pcall(foo, 2, 3) >> result;
	BOOST_TEST(result == 5);
}

BOOST_AUTO_TEST_CASE(testCallFunctionVar2)
{
	Lua::Engine lua;

	Lua::Function foo = Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			int x, y;
			args.in() >> x >> y;
			args.out() << x + y;
		}
	);

	Lua::Function bar = Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			Lua::Function foo;
			args.in() >> foo;

			lua.args().in() << args.in();
			lua.args().in() << args.in();

			lua.pcall(foo) >> args.out();
		}
	);

	int result = 0;
	lua.pcall(foo, 2, 3) >> result;
	BOOST_TEST(result == 5);
}

BOOST_AUTO_TEST_CASE(testCallFunctionVar3)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			Lua::Function bar;
			args.in() >> bar;

			args.out() << Lua::MakeFunc(
				[bar](Lua::Args args, Lua::Lua lua) mutable
				{
					lua.args().in() << args.in();
					lua.args().in() << args.in();

					lua.pcall(bar) >> args.out();
				}
			);
		}
	);

	lua.args().in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			int x, y;
			args.in() >> x >> y;
			args.out() << x + y;
		}
	);

	Lua::Function bar;
	lua.pcall("foo") >> bar;

	int result = 0;
	lua.pcall(bar, 2, 3) >> result;
	BOOST_TEST(result == 5);
}

BOOST_AUTO_TEST_CASE(testCallFunctionVar4)
{
	Lua::Engine lua;

	lua.global("foo").in() << Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			int x, y;
			args.in() >> x >> y;
			args.out() << x + y;
		}
	);

	Lua::Function foo;
	lua.global("foo").out() >> foo;
	BOOST_TEST(lua.state().getL() == foo.getL());

	{
		BOOST_TEST(Lua::Library::inst().usecount(lua.state().getL()) == 1);
		Lua::AuxiliaryContext aux(foo.getL());
		BOOST_TEST(Lua::Library::inst().usecount(lua.state().getL()) == 2);

		int result = 0;
		aux.pcall(foo, 2, 3) >> result;
		BOOST_TEST(result == 5);
	}

	BOOST_TEST(Lua::Library::inst().usecount(lua.state().getL()) == 1);
}

BOOST_AUTO_TEST_CASE(testCallFunctionVar5)
{
	Lua::Function foo;

	{
		Lua::Engine lua;

		lua.global("foo").in() << Lua::MakeFunc(
			[](Lua::Args args, Lua::Lua lua)
			{
				int x, y;
				args.in() >> x >> y;
				args.out() << x + y;
			}
		);

		BOOST_TEST(Lua::Library::inst().usecount(foo.getL()) == 0);
		lua.global("foo").out() >> foo;
		BOOST_TEST(Lua::Library::inst().usecount(foo.getL()) == 1);
	}

	BOOST_TEST(Lua::Library::inst().usecount(foo.getL()) == 0);

	Lua::AuxiliaryContext aux(foo.getL());

	std::string what;

	try {
		aux.pcall(foo);
	}
	catch (std::exception& e) {
		what = e.what();
	}

	BOOST_TEST(what == "invalid engine state");
}

BOOST_AUTO_TEST_SUITE_END()