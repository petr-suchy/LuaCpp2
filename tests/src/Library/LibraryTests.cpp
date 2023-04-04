#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include <LuaCpp2/Library.h>
#include <sstream>

static int testedFunction(Lua::Library::State* L)
{
	Lua::Library::Integer x = Lua::Library::inst().tointeger(L, 1);
	Lua::Library::Integer y = Lua::Library::inst().tointeger(L, 2);

	Lua::Library::inst().pushinteger(L, (x + y));

	return 1;
}

static int testedFunctionWithError(Lua::Library::State* L)
{

	Lua::Library::inst().pushstring(L, "test error string");
	Lua::Library::inst().error(L);

	return 0;
}

static int testedClosure(Lua::Library::State* L)
{
	Lua::Library::Integer n = Lua::Library::inst().tointeger(
		L, Lua::Library::inst().upvalueindex(1)
	);
	
	Lua::Library::inst().pushinteger(L, n);

	return 1;
}

static const char* testedReader(Lua::Library::State* L, void* ud, size_t* sz)
{
	static char buff[512];

	std::istringstream* iss = reinterpret_cast<
		std::istringstream*
	>(ud);

	iss->read(buff, sizeof(buff));
	*sz = (size_t) iss->gcount();

	return buff;
}

static void testedStateInitializer(Lua::Library::State* L)
{
	Lua::Library::inst().pushinteger(L, 123456);
	Lua::Library::inst().setglobal(L, "fromInitializer");
}

static int testedWriter(Lua::Library::State* L, const void* p, size_t sz, void* ud)
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

BOOST_AUTO_TEST_CASE(testVersion)
{
	auto ifceVer = Lua::Library::inst().getifcever();

	BOOST_TEST(ifceVer.Major == Lua::Library::InterfaceVersionMajor);
	BOOST_TEST(ifceVer.Minor == Lua::Library::InterfaceVersionMinor);
}

BOOST_AUTO_TEST_CASE(testNewState)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	BOOST_TEST(L != nullptr);
	BOOST_TEST(Lua::Library::inst().usecount(L) == 1);

	Lua::Library::inst().close(L);
	BOOST_TEST(Lua::Library::inst().usecount(L) == 0);
}

BOOST_AUTO_TEST_CASE(testStateInitializer)
{
	Lua::Library::inst().setstateinit(&testedStateInitializer);
	BOOST_TEST(Lua::Library::inst().getstateinit() == &testedStateInitializer);

	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().getglobal(L, "fromInitializer");
	BOOST_TEST(Lua::Library::inst().tointeger(L, 1) == 123456);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testLockState)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	BOOST_TEST(L != nullptr);
	BOOST_TEST(Lua::Library::inst().usecount(L) == 1);

	BOOST_TEST(Lua::Library::inst().lockstate(L) != nullptr);
	BOOST_TEST(Lua::Library::inst().usecount(L) == 2);

	Lua::Library::inst().close(L);
	BOOST_TEST(Lua::Library::inst().usecount(L) == 1);

	Lua::Library::inst().close(L);
	BOOST_TEST(Lua::Library::inst().usecount(L) == 0);
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

BOOST_AUTO_TEST_CASE(testNext)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	std::map<std::string, std::string> tbl1 = {
		{"Mon", "Monday"}, {"Tue", "Tuesday"}, {"Wed", "Wednesday"},
		{"Thu", "Thursday"}, {"Fri", "Friday"}, {"Sat", "Saturday "},
		{"Sun", "Sunday"}
	};

	Lua::Library::inst().createtable(L, 0, 0);

	for (auto& day : tbl1) {
		Lua::Library::inst().pushstring(L, day.first.c_str());
		Lua::Library::inst().pushstring(L, day.second.c_str());
		Lua::Library::inst().settable(L, 1);
	}

	std::map<std::string, std::string> tbl2;

	Lua::Library::inst().pushnil(L);

	while (Lua::Library::inst().next(L, 1)) {

		tbl2.insert({
				Lua::Library::inst().tostring(L, -2),
				Lua::Library::inst().tostring(L, -1)
			});

		Lua::Library::inst().pop(L, 1);
	}

	BOOST_TEST(tbl1 == tbl2);

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

	Lua::Library::inst().pushcfunction(L, &testedFunction);
	Lua::Library::inst().pushinteger(L, 2);
	Lua::Library::inst().pushinteger(L, 3);
	Lua::Library::inst().pcall(L, 2);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().tointeger(L, 1) == 5);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testFunctionWithError)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushcfunction(L, &testedFunctionWithError);
	int status = Lua::Library::inst().pcall(L, 0);

	BOOST_TEST(status != 0);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);

	char buff[256];
	size_t len = Lua::Library::inst().toerrorstring(L, status, buff, sizeof(buff));

	BOOST_TEST(Lua::Library::inst().gettop(L) == 0);

	std::string error(buff, len);
	BOOST_TEST(error == "test error string");

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testClosure)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushinteger(L, 123);
	Lua::Library::inst().pushcclosure(L, &testedClosure, 1);
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

	Lua::Library::inst().load(L, &testedReader, &iss, "", "bt");

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().isfunction(L, 1));

	Lua::Library::inst().pcall(L, 0);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().isfunction(L, 1));

	// dump chunk

	std::stringstream ss;

	Lua::Library::inst().dump(L, &testedWriter, &ss, 0);
	
	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST((ss.str().length() > 0));
	
	// dump does not pop the function from the stack
	Lua::Library::inst().pop(L, 1);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 0);
	
	Lua::Library::inst().load(L, &testedReader, &ss, "", "bt");

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

BOOST_AUTO_TEST_CASE(testUserdata)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	void* p1 = Lua::Library::inst().newuserdata(L, 256);

	BOOST_TEST(p1);
	BOOST_TEST(Lua::Library::inst().isuserdata(L, 1));

	void* p2 = Lua::Library::inst().touserdata(L, 1);

	BOOST_TEST(p1 == p2);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testReference)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushinteger(L, 123);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().isinteger(L, 1));

	int ref = Lua::Library::inst().ref(L);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 0);
	BOOST_TEST(!Lua::Library::inst().isrefnil(ref));

	Lua::Library::inst().getref(L, ref);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().isinteger(L, 1));
	BOOST_TEST(Lua::Library::inst().tointeger(L, 1) == 123);

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testNilReference)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushnil(L);
	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().isnil(L, 1));

	int ref = Lua::Library::inst().ref(L);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 0);
	BOOST_TEST(Lua::Library::inst().isrefnil(ref));

	Lua::Library::inst().getref(L, ref);

	BOOST_TEST(Lua::Library::inst().gettop(L) == 1);
	BOOST_TEST(Lua::Library::inst().isnil(L, 1));

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testIntTypeToName)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushnil(L);
	Lua::Library::inst().pushboolean(L, 1);
	Lua::Library::inst().pushnumber(L, 123);
	Lua::Library::inst().pushstring(L, "Hi!");
	Lua::Library::inst().createtable(L, 0, 0);
	Lua::Library::inst().pushcfunction(L, &testedFunction);
	Lua::Library::inst().newuserdata(L, 4);

	BOOST_TEST(Lua::Library::inst().typetoname(L, Lua::Library::inst().type(L, 1)) == "nil");
	BOOST_TEST(Lua::Library::inst().typetoname(L, Lua::Library::inst().type(L, 2)) == "boolean");
	BOOST_TEST(Lua::Library::inst().typetoname(L, Lua::Library::inst().type(L, 3)) == "number");
	BOOST_TEST(Lua::Library::inst().typetoname(L, Lua::Library::inst().type(L, 4)) == "string");
	BOOST_TEST(Lua::Library::inst().typetoname(L, Lua::Library::inst().type(L, 5)) == "table");
	BOOST_TEST(Lua::Library::inst().typetoname(L, Lua::Library::inst().type(L, 6)) == "function");
	BOOST_TEST(Lua::Library::inst().typetoname(L, Lua::Library::inst().type(L, 7)) == "userdata");

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testEnumTypeToName)
{
	BOOST_TEST(Lua::Library::inst().typetoname(Lua::Library::Type::Boolean) == "boolean");
	BOOST_TEST(Lua::Library::inst().typetoname(Lua::Library::Type::CFunction) == "cfunction");
	BOOST_TEST(Lua::Library::inst().typetoname(Lua::Library::Type::Function) == "function");
	BOOST_TEST(Lua::Library::inst().typetoname(Lua::Library::Type::Integer) == "integer");
	BOOST_TEST(Lua::Library::inst().typetoname(Lua::Library::Type::LightUserdata) == "lightuserdata");
	BOOST_TEST(Lua::Library::inst().typetoname(Lua::Library::Type::Nil) == "nil");
	BOOST_TEST(Lua::Library::inst().typetoname(Lua::Library::Type::Number) == "number");
	BOOST_TEST(Lua::Library::inst().typetoname(Lua::Library::Type::String) == "string");
	BOOST_TEST(Lua::Library::inst().typetoname(Lua::Library::Type::Table) == "table");
	BOOST_TEST(Lua::Library::inst().typetoname(Lua::Library::Type::Thread) == "thread");
	BOOST_TEST(Lua::Library::inst().typetoname(Lua::Library::Type::Userdata) == "userdata");
}

BOOST_AUTO_TEST_CASE(testIsValue)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().pushboolean(L, 1);
	Lua::Library::inst().pushcfunction(L, &testedFunction);

	// push a function
	std::istringstream iss("return 123");
	Lua::Library::inst().load(L, &testedReader, &iss, "", "t");

	Lua::Library::inst().pushinteger(L, 123);
	Lua::Library::inst().pushnil(L);
	Lua::Library::inst().pushnumber(L, 123.45);
	Lua::Library::inst().pushstring(L, "Hi!");
	Lua::Library::inst().createtable(L, 0, 0);
	Lua::Library::inst().newuserdata(L, 4);

	BOOST_TEST((Lua::Library::inst().isvalue(L, 1, Lua::Library::Type::Boolean)));
	BOOST_TEST((Lua::Library::inst().isvalue(L, 2, Lua::Library::Type::CFunction)));
	BOOST_TEST((Lua::Library::inst().isvalue(L, 3, Lua::Library::Type::Function)));
	BOOST_TEST((Lua::Library::inst().isvalue(L, 4, Lua::Library::Type::Integer)));
	BOOST_TEST((Lua::Library::inst().isvalue(L, 5, Lua::Library::Type::Nil)));
	BOOST_TEST((Lua::Library::inst().isvalue(L, 6, Lua::Library::Type::Number)));
	BOOST_TEST((Lua::Library::inst().isvalue(L, 7, Lua::Library::Type::String)));
	BOOST_TEST((Lua::Library::inst().isvalue(L, 8, Lua::Library::Type::Table)));
	BOOST_TEST((Lua::Library::inst().isvalue(L, 9, Lua::Library::Type::Userdata)));

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_CASE(testOpenlibs)
{
	Lua::Library::State* L = Lua::Library::inst().newstate();

	Lua::Library::inst().openlibs(L);

	Lua::Library::inst().getglobal(L, "print");
	BOOST_TEST(Lua::Library::inst().isfunction(L, 1));

	Lua::Library::inst().close(L);
}

BOOST_AUTO_TEST_SUITE_END()