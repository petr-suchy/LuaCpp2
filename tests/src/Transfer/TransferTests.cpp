#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

// for transferring booleans
#include <LuaCpp2/ReadableBoolean.h>
#include <LuaCpp2/WritableBoolean.h>

// for transferring integers
#include <LuaCpp2/ReadableInteger.h>
#include <LuaCpp2/WritableInteger.h>

// for transferring numbers
#include <LuaCpp2/ReadableNumber.h>
#include <LuaCpp2/WritableNumber.h>

// for transferring strings
#include <LuaCpp2/ReadableString.h>
#include <LuaCpp2/WritableString.h>

// for transferring tables
#include <LuaCpp2/Table.h>
#include <LuaCpp2/Field.h>

#include <LuaCpp2/TransferStack.h>

BOOST_AUTO_TEST_SUITE(Transfer__Transfer)

BOOST_AUTO_TEST_CASE(testOutputStack)
{
	// it should write some items to the stack
	// via OutputStack abstraction

	Lua::State state;
	state.open();
	BOOST_TEST(state.isOpen());

	{
		Lua::OutputStack ostack(state);

		ostack.insertNil();
		ostack.insertBoolean(true);
		ostack.insertInteger(123);
		ostack.insertNumber(123.5);
		ostack.insertString("xxx");
		ostack.insertTable();

		ostack.finish();
	}

	BOOST_TEST(state.getStackTop() == 6);

	BOOST_TEST(state.isNilAt(1));
	BOOST_TEST(state.isBooleanAt(2));
	BOOST_TEST(state.isIntegerAt(3));
	BOOST_TEST((Lua::Library::inst().isnumber(state.getL(), 4) != 0));
	BOOST_TEST((Lua::Library::inst().isstring(state.getL(), 5) != 0));
	BOOST_TEST((Lua::Library::inst().istable(state.getL(), 6) != 0));
}

BOOST_AUTO_TEST_CASE(testOutputStackIntegrity)
{
	// if OutputStack::finish() has not been called,
	// it should discard the written items on the stack

	Lua::State state;
	state.open();
	BOOST_TEST(state.isOpen());

	{
		Lua::OutputStack ostack(state);

		ostack.insertNil();
		ostack.insertBoolean(true);
		ostack.insertInteger(123);
		ostack.insertNumber(123.5);
		ostack.insertString("xxx");
		ostack.insertTable();

		// ostack.finish()
	}

	BOOST_TEST(state.getStackTop() == 0);
}

BOOST_AUTO_TEST_CASE(testBoolean)
{
	Lua::State local;
	local.open();
	BOOST_TEST(local.isOpen());

	Lua::State remote;
	remote.open();
	BOOST_TEST(remote.isOpen());

	local << Lua::WritableBoolean(true);

	Lua::Transfer(local, remote);

	BOOST_TEST(remote.getStackTop() == 1);

	BOOST_TEST(remote.isBooleanAt(Lua::State::StackTop));

	bool boolVal = false;
	Lua::ReadableBoolean rb(boolVal);
	remote >> rb;

	BOOST_TEST(boolVal);
}

BOOST_AUTO_TEST_CASE(testInteger)
{
	Lua::State local;
	local.open();
	BOOST_TEST(local.isOpen());

	Lua::State remote;
	remote.open();
	BOOST_TEST(remote.isOpen());

	local << Lua::WritableInteger<int>(123);

	Lua::Transfer(local, remote);

	BOOST_TEST(remote.getStackTop() == 1);
	BOOST_TEST(remote.isIntegerAt(Lua::State::StackTop));

	int i = 0;
	Lua::ReadableInteger<int> ri(i);
	remote >> ri;

	BOOST_TEST(i == 123);
}

BOOST_AUTO_TEST_CASE(testNumber)
{
	Lua::State local;
	local.open();
	BOOST_TEST(local.isOpen());

	Lua::State remote;
	remote.open();
	BOOST_TEST(remote.isOpen());

	local << Lua::WritableNumber<float>(123.5);

	Lua::Transfer(local, remote);

	BOOST_TEST(remote.getStackTop() == 1);

	BOOST_TEST((Lua::Library::inst().isnumber(remote.getL(), remote.getStackTop()) != 0));

	float n = 0;
	Lua::ReadableNumber<float> rn(n);
	remote >> rn;

	BOOST_TEST(n == 123.5);
}

BOOST_AUTO_TEST_CASE(testString)
{
	Lua::State local;
	local.open();
	BOOST_TEST(local.isOpen());

	Lua::State remote;
	remote.open();
	BOOST_TEST(remote.isOpen());

	const std::string str1 = "Hello!";
	local << Lua::WritableString<std::string>(str1);

	Lua::Transfer(local, remote);

	BOOST_TEST(remote.getStackTop() == 1);

	BOOST_TEST((Lua::Library::inst().isstring(remote.getL(), remote.getStackTop()) != 0));

	std::string str2;
	Lua::ReadableString<std::string> rs(str2);
	remote >> rs;

	BOOST_TEST(str1 == str2);
}

BOOST_AUTO_TEST_CASE(testTable)
{
	Lua::State local;
	local.open();
	BOOST_TEST(local.isOpen());

	Lua::State remote;
	remote.open();
	BOOST_TEST(remote.isOpen());

	// write the local table to the local stack

	const bool b1 = true;
	const int i1 = 123;
	const float n1 = 123.5;
	const std::string s1 = "xxx";

	{
		Lua::WritableStackSlot slot(local);

		slot.prepare();
		{
			{
				Lua::Table tbl(slot.insertTable());
						
				tbl.state()
					<< Lua::Field("b") << Lua::WritableBoolean(b1)
					<< Lua::Field("i") << Lua::WritableInteger<int>(i1)
					<< Lua::Field("n") << Lua::WritableNumber<float>(n1)
					<< Lua::Field("s") << Lua::WritableString<std::string>(s1);
						
			}
		}
		slot.finish();
	}

	// transfer the local table to the remote stack
	Lua::Transfer(local, remote);
			
	BOOST_TEST(remote.getStackTop() == 1);

	BOOST_TEST((Lua::Library::inst().istable(remote.getL(), local.getStackTop()) != 0));

	// read the remote table

	bool b2 = false;
	int i2 = 0;
	float n2 = 0;
	std::string s2;

	{
		Lua::ReadableStackSlot slot(remote);

		slot.prepare(Lua::Library::Type::Table);
		{
			{
				Lua::Table tbl(slot.getTable());
						
				Lua::ReadableBoolean rb(b2);
				Lua::ReadableInteger<int> ri(i2);
				Lua::ReadableNumber<float> rn(n2);
				Lua::ReadableString<std::string> rs(s2);

				tbl.state()
					>> Lua::Field("b") >> rb
					>> Lua::Field("i") >> ri
					>> Lua::Field("n") >> rn
					>> Lua::Field("s") >> rs;
						
			}
		}
		slot.finish();
	}

	BOOST_TEST(b2);
	BOOST_TEST(i1 == i2);
	BOOST_TEST(n1 == n2);
	BOOST_TEST(s1 == s2);
}


BOOST_AUTO_TEST_SUITE_END()