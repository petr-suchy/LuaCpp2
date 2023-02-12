#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include "../../../include/ReadableString.h"
#include "../../../include/WritableString.h"

#include "../../../include/WritableWString.h"
#include "../../../include/ReadableWString.h"

#include <codecvt>

BOOST_AUTO_TEST_SUITE(State__Strings)

BOOST_AUTO_TEST_CASE(testString)
{
	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	std::string str = "abcdefghijklmnopqrstuvwxyz";
	state << Lua::WritableString<std::string>(str);

	std::string str2;
	Lua::ReadableString<std::string> rs(str2);
	state >> rs;

	BOOST_TEST(str == str2);
}

BOOST_AUTO_TEST_CASE(testWideString)
{

	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	std::wstring str = L"nìjaký èlovìk šlápl na øepu u kùlu";

	state << Lua::WritableWString<
		std::wstring,
		std::wstring_convert<
		std::codecvt_utf8<wchar_t>
		>
	>(str);

	std::wstring str2;

	Lua::ReadableWString<
		std::wstring,
		std::wstring_convert<
			std::codecvt_utf8<wchar_t>
		>
	> rws(str2);

	state >> rws;

	bool areEqual = (str == str2);

	BOOST_TEST(areEqual);
}

BOOST_AUTO_TEST_SUITE_END()