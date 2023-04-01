#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include <LuaCpp2/LuaCpp2.h>

BOOST_AUTO_TEST_SUITE(Engine__FunctionCall)

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
					"unexpected value of the first input argument"
				);
			}

			int n2;
			args.in() >> n2;

			if (n2 != 4) {
				throw std::logic_error(
					"unexpected value of the second input argument"
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
	Lua::Engine lua(
		[](Lua::Lua lua)
		{
			lua.global("forward").in() << Lua::MakeFunc(
				[](Lua::Args args, Lua::Lua lua)
				{
					Lua::Function f;
					args.in() >> f;

					while (!args.in().atEnd()) {
						lua.args().in() << args.in();
					}

					lua.pcall(f);

					while (!lua.args().out().atEnd()) {
						lua.args().out() >> args.out();
					}
				}
			);
		}
	);

	auto implode = Lua::MakeFunc(
		[](Lua::Args args, Lua::Lua lua)
		{
			std::stringstream ss;
			int n;

			while (!args.in().atEnd()) {

				args.in() >> n;
				ss << n;

				if (!args.in().atEnd()) {
					ss << ", ";
				}
			}

			args.out() << ss.str();
		}
	);

	std::string result;
	lua.pcall("forward", implode, 10, 20, 30, 40) >> result;

	BOOST_TEST(result == "10, 20, 30, 40");
}

BOOST_AUTO_TEST_SUITE_END()