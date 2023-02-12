#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include "../../../include/luacpp/Table.h"
#include "../../../include/luacpp/Fields.h"

#include "../../../include/luacpp/ReadableInteger.h"
#include "../../../include/luacpp/WritableInteger.h"
#include "../../../include/luacpp/ReadableString.h"
#include "../../../include/luacpp/WritableString.h"

BOOST_AUTO_TEST_SUITE(State__Tables)

BOOST_AUTO_TEST_CASE(testNestedTable)
{
	const int num = 123456;
	const int nestedTblNum = 111111;
	const std::string str = "string";
	const std::string nestedTblStr = "nestedTableString";

	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		Lua::WritableStackSlot slot(state);

		slot.prepare();
		{
			Lua::Table tbl(slot.insertTable());
			BOOST_TEST(state.getTableLevel() == 1);

			tbl.state()
				<< Lua::Field("num") << Lua::WritableInteger<int>(num)
				<< Lua::Field("str") << Lua::WritableString<std::string>(str)
				<< Lua::Field("tbl");

			{
				Lua::WritableStackSlot slot2(tbl.state());
						
				slot2.prepare();
				{
					Lua::Table tbl2(slot2.insertTable());
					BOOST_TEST(state.getTableLevel() == 2);

					tbl2.state()
						<< Lua::Field("num") << Lua::WritableInteger<int>(nestedTblNum)
						<< Lua::Field("str") << Lua::WritableString<std::string>(nestedTblStr);

				}
				BOOST_TEST(state.getTableLevel() == 1);
				slot2.finish();
			}

		}
		BOOST_TEST(state.getTableLevel() == 0);
		slot.finish();

	}

	BOOST_TEST(state.getStackTop() == 1);
	BOOST_TEST(state.getType(state.getStackTop()) == LUA_TTABLE);

	int num2 = 0;
	int nestedTblNum2 = 0;
	std::string str2;
	std::string nestedTblStr2;

	{
		Lua::ReadableStackSlot slot(state);

		slot.prepare(LUA_TTABLE);
		{
			Lua::Table tbl(slot.getTable());
			BOOST_TEST(state.getTableLevel() == 1);

			Lua::ReadableInteger<int> rn(num2);
			Lua::ReadableString<std::string> rs(str2);

			tbl.state()
				>> Lua::Field("num") >> rn
				>> Lua::Field("str") >> rs
				>> Lua::Field("tbl");

			{
				Lua::ReadableStackSlot slot2(tbl.state());

				slot2.prepare(LUA_TTABLE);
				{
					Lua::Table tbl2(slot2.getTable());
					BOOST_TEST(state.getTableLevel() == 2);


					Lua::ReadableInteger<int> rn2(nestedTblNum2);
					Lua::ReadableString<std::string> rs2(nestedTblStr2);

					tbl2.state()
						>> Lua::Field("num") >> rn2
						>> Lua::Field("str") >> rs2;

				}
				BOOST_TEST(state.getTableLevel() == 1);
				slot2.finish();
			}

		}
		BOOST_TEST(state.getTableLevel() == 0);
		slot.finish();

	}

	BOOST_TEST(state.getStackTop() == 0);

	BOOST_TEST(num == num2);
	BOOST_TEST(str == str2);

	BOOST_TEST(nestedTblNum == nestedTblNum2);
	BOOST_TEST(nestedTblStr == nestedTblStr2);
}

BOOST_AUTO_TEST_CASE(testTableFields)
{
	const int num = 123456;
	const std::string str = "string";

	Lua::State state;

	state.open();
	BOOST_TEST(state.isOpen());

	{
		Lua::WritableStackSlot slot(state);

		slot.prepare();
		{
			Lua::Table tbl(slot.insertTable());
			BOOST_TEST(state.getTableLevel() == 1);

			tbl.state() << Lua::Fields("num, str")
				<< Lua::WritableInteger<int>(num)
				<< Lua::WritableString<std::string>(str);

		}
		BOOST_TEST(state.getTableLevel() == 0);
		slot.finish();
	}

	BOOST_TEST(state.getStackTop() == 1);
	BOOST_TEST(state.getType(state.getStackTop()) == LUA_TTABLE);

	int num2 = 0;
	std::string str2;

	{
		Lua::ReadableStackSlot slot(state);

		slot.prepare(LUA_TTABLE);
		{
			Lua::Table tbl(slot.getTable());
			BOOST_TEST(state.getTableLevel() == 1);

			Lua::ReadableInteger<int> rn(num2);
			Lua::ReadableString<std::string> rs(str2);

			tbl.state() >> Lua::Fields("num, str")
				>> rn >> rs;

		}
		BOOST_TEST(state.getTableLevel() == 0);
		slot.finish();
	}

	BOOST_TEST(state.getStackTop() == 0);

	BOOST_TEST(num == num2);
	BOOST_TEST(str == str2);
}

BOOST_AUTO_TEST_SUITE_END()
