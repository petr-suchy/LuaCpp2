#define BOOST_TEST_MODULE Tests
#include <boost/test/unit_test.hpp>

#include "lua.hpp"
#include <LuaCpp2/LinkedLibrary.h>

class NewState {
public:

    Lua::Library::State* operator()()
    {
        return luaL_newstate();
    }

};

Lua::Library& Lua::Library::inst()
{
    static LinkedLibrary<NewState> lib;
    return lib;
}