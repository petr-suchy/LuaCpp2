#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include <LuaCpp2/Library.h>
#include <sstream>

static int testingFunction(Lua::Library::State* L)
{
	Lua::Library::Integer x = Lua::Library::inst().tointeger(L, 1);
	Lua::Library::Integer y = Lua::Library::inst().tointeger(L, 2);

	Lua::Library::inst().pushinteger(L, (x + y));

	return 1;
}

static int testingClosure(Lua::Library::State* L)
{
	Lua::Library::Integer n = Lua::Library::inst().tointeger(
		L, Lua::Library::inst().upvalueindex(1)
	);
	
	Lua::Library::inst().pushinteger(L, n);

	return 1;
}

static const char* testingReader(Lua::Library::State* L, void* ud, size_t* sz)
{
	static char buff[512];

	std::istringstream* iss = reinterpret_cast<
		std::istringstream*
	>(ud);

	iss->read(buff, sizeof(buff));
	*sz = (size_t) iss->gcount();

	return buff;
}

static int testingWriter(Lua::Library::State* L, const void* p, size_t sz, void* ud)
{
	std::ostringstream* oss = reinterpret_cast<
		std::ostringstream*
	>(ud);

	oss->write(
		reinterpret_cast<const char*>(p), sz
	);

	return 0;
}

BOOST_AUTO_TEST_SUITE(Library__Library)

BOOST_AUTO_TEST_CASE(testNewState)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();
	BOOST_TEST(L != nullptr);
	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testStackTop)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	BOOST_TEST(Lua::Library::inst().gettop(L) == 0);
	BOOST_TEST(Lua::Library::inst().checkstack(L, 3));

	Lua::Library::inst().pushinteger(L, 10);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);

	Lua::Library::inst().pushinteger(L, 20);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 2);

	Lua::Library::inst().pushinteger(L, 30);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 3);

	Lua::Library::inst().settop(L, 0);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 0);

	Lua::Library::inst().settop(L, 3);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 3);

	BOOST_TEST(Lua::Library::inst().isnil(L, 1));
	BOOST_TEST(Lua::Library::inst().isnil(L, 2));
	BOOST_TEST(Lua::Library::inst().isnil(L, 3));

	Lua::Library::inst().pop(L, 2);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);

	Lua::Library::inst().pop(L, 1);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 0);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testPushValue)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushinteger(L, 10);
	Lua::Library::inst().pushvalue(L, 1);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 2);

	BOOST_TEST(Lua::Library::inst().tointeger(L, 1) == 10);
	BOOST_TEST(Lua::Library::inst().tointeger(L, 2) == 10);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testRemove)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushinteger(L, 10);
	Lua::Library::inst().pushinteger(L, 20);
	Lua::Library::inst().pushinteger(L, 30);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 3);

	Lua::Library::inst().remove(L, 2);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 2);

	BOOST_TEST(Lua::Library::inst().tointeger(L, 1) == 10);
	BOOST_TEST(Lua::Library::inst().tointeger(L, 2) == 30);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testInsert)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushinteger(L, 10);
	Lua::Library::inst().pushinteger(L, 30);
	Lua::Library::inst().pushinteger(L, 20);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 3);

	Lua::Library::inst().insert(L, 2);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 3);

	BOOST_TEST(Lua::Library::inst().tointeger(L, 1) == 10);
	BOOST_TEST(Lua::Library::inst().tointeger(L, 2) == 20);
	BOOST_TEST(Lua::Library::inst().tointeger(L, 3) == 30);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testReplace)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushinteger(L, 10);
	Lua::Library::inst().pushinteger(L, 20);
	Lua::Library::inst().pushinteger(L, 30);
	Lua::Library::inst().pushinteger(L, 40);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 4);

	Lua::Library::inst().replace(L, 2);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 3);

	BOOST_TEST(Lua::Library::inst().tointeger(L, 1) == 10);
	BOOST_TEST(Lua::Library::inst().tointeger(L, 2) == 40);
	BOOST_TEST(Lua::Library::inst().tointeger(L, 3) == 30);

	Lua::Library::inst().close(L);
}
BOOST_AUTO_TEST_CASE(testBoolean)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushboolean(L, 0);
	Lua::Library::inst().pushboolean(L, 1);

	BOOST_TEST(Lua::Library::inst().isboolean(L, -1));
	BOOST_TEST(Lua::Library::inst().isboolean(L, -2));

	BOOST_TEST(Lua::Library::inst().toboolean(L, -1));
	BOOST_TEST(!Lua::Library::inst().toboolean(L, -2));

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testNil)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushnil(L);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().isnil(L, -1));

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testNumber)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushnumber(L, 123.5);

	BOOST_TEST(Lua::Library::inst().isnumber(L, -1));
	BOOST_TEST(Lua::Library::inst().tonumber(L, -1) == 123.5);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testInteger)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushinteger(L, 12345);

	BOOST_TEST(Lua::Library::inst().isinteger(L, -1));
	BOOST_TEST(Lua::Library::inst().tointeger(L, -1) == 12345);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testString)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushstring(L, "Hi!");
	Lua::Library::inst().pushlstring(L, "ABCDEF", 3);

	BOOST_TEST(Lua::Library::inst().isstring(L, -1));
	BOOST_TEST(Lua::Library::inst().isstring(L, -2));

	BOOST_TEST(Lua::Library::inst().tostring(L, -2) == "Hi!");

	size_t len = 0;
	BOOST_TEST(Lua::Library::inst().tolstring(L, -1, &len) == "ABC");
	BOOST_TEST(len == 3);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testTable)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().createtable(L, 0, 0);

	// t['num'] = 123
	Lua::Library::inst().pushstring(L, "num");
	Lua::Library::inst().pushinteger(L, 123);
	Lua::Library::inst().settable(L, -3);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().istable(L, 1));

	// t['str'] = "Hi!"
	Lua::Library::inst().pushstring(L, "Hi!");
	Lua::Library::inst().setfield(L, -2, "str");

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().istable(L, 1));

	// get field 'num'
	Lua::Library::inst().pushstring(L, "num");
	Lua::Library::inst().gettable(L, -2);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 2);
	BOOST_TEST(Lua::Library::inst().tointeger(L, -1) == 123);
	Lua::Library::inst().pop(L, 1);

	// get field 'str'
	Lua::Library::inst().getfield(L, 1, "str");

	BOOST_TEST(Lua::Library::inst().gettop(L) == 2);
	BOOST_TEST(Lua::Library::inst().tostring(L, -1) == "Hi!");
	Lua::Library::inst().pop(L, 1);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testMetatable)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().createtable(L, 0, 0);

	BOOST_TEST(!Lua::Library::inst().getmetatable(L, 1));
	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);

	Lua::Library::inst().createtable(L, 0, 0);
	Lua::Library::inst().setmetatable(L, 1);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);

	BOOST_TEST(Lua::Library::inst().getmetatable(L, 1));
	BOOST_TEST(Lua::Library::inst().gettop(L) == 2);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testGlobal)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushinteger(L, 123);
	Lua::Library::inst().setglobal(L, "num");
	BOOST_TEST(Lua::Library::inst().gettop(L) == 0);

	Lua::Library::inst().pushstring(L, "Hi!");
	Lua::Library::inst().setglobal(L, "str");
	BOOST_TEST(Lua::Library::inst().gettop(L) == 0);

	Lua::Library::inst().getglobal(L, "num");
	BOOST_TEST(Lua::Library::inst().isinteger(L, -1));
	BOOST_TEST(Lua::Library::inst().tointeger(L, -1) == 123);

	Lua::Library::inst().getglobal(L, "str");
	BOOST_TEST(Lua::Library::inst().isstring(L, -1));
	BOOST_TEST(Lua::Library::inst().tostring(L, -1) == "Hi!");

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testFunction)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushcfunction(L, &testingFunction);
	Lua::Library::inst().pushinteger(L, 2);
	Lua::Library::inst().pushinteger(L, 3);
	Lua::Library::inst().pcall(L, 2);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().tointeger(L, 1) == 5);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testClosure)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushinteger(L, 123);
	Lua::Library::inst().pushcclosure(L, &testingClosure, 1);
	Lua::Library::inst().pcall(L, 0);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().tointeger(L, 1) == 123);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testChunk)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();
	
	// load chunk

	std::istringstream iss(
		"return function (x, y) return x + y end"
	);

	Lua::Library::inst().load(L, &testingReader, &iss, "", "bt");

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().isfunction(L, 1));

	Lua::Library::inst().pcall(L, 0);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().isfunction(L, 1));

	// dump chunk

	std::stringstream ss;

	Lua::Library::inst().dump(L, &testingWriter, &ss, 0);
	
	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST((ss.str().length() > 0));
	
	// dump does not pop the function from the stack
	Lua::Library::inst().pop(L, 1);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 0);
	
	Lua::Library::inst().load(L, &testingReader, &ss, "", "bt");

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().isfunction(L, 1));
	
	// call returned function

	Lua::Library::inst().pushinteger(L, 2);
	Lua::Library::inst().pushinteger(L, 3);
	Lua::Library::inst().pcall(L, 2);

	// check result

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().isinteger(L, 1));
	BOOST_TEST(Lua::Library::inst().tointeger(L, 1) == 5);
	
	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_SUITE_END()