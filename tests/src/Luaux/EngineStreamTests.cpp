#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include <LuaCpp2/Luaux.h>

BOOST_AUTO_TEST_SUITE(Luaux__EngineStreams)

BOOST_AUTO_TEST_CASE(testStringStream)
{
    Lua::Engine lua(
        [](Lua::Lua lua)
        {
            lua.openStdLibs();

            lua.global("StringStream").in() << Lua::MakeFunc(
                [](Lua::Args args, Lua::Lua lua)
                {
                    std::string str;
                    args.in() >> str;
                    auto ss = std::make_shared<std::stringstream>(str);
                    args.out() << Luaux::Engine::MakeStringStream(ss);
                }
            );

        }
    );

    lua.doString(
        // 1
        "local ss = StringStream('ABCDEFGHIJKLM')"
        "assert(ss.tellg() == 0)"
        "assert(ss.tellp() == 0)\n"
        // 2
        "ss.seekg(0, ss.endp)"
        "ss.seekp(0, ss.endp)"
        "assert(ss.tellg() == 13)"
        "assert(ss.tellp() == 13)\n"
        // 3
        "ss.seekg(0, ss.begp)"
        "ss.seekp(0, ss.begp)"
        "assert(ss.tellg() == 0)"
        "assert(ss.tellp() == 0)\n"
        // 4
        "local buff, eof = ss.read(5)"
        "assert(buff == 'ABCDE')"
        "assert(not eof)"
        "assert(ss.tellg() == 5)"
        "assert(ss.tellp() == 0)\n"
        // 5
        "buff, eof = ss.read(5)"
        "assert(buff == 'FGHIJ')"
        "assert(not eof)"
        "assert(ss.tellg() == 10)"
        "assert(ss.tellp() == 0)\n"
        // 6
        "buff, eof = ss.read(5)"
        "assert(buff == 'KLM')"
        "assert(eof)"
        "assert(ss.tellg() == -1)"
        "assert(ss.tellp() == -1)\n"
        // 7
        "ss.clear()"
        "assert(ss.tellg() == 13)"
        "assert(ss.tellp() == 0)\n"
        // 8
        "ss.seekp(0, ss.endp)"
        "assert(ss.tellg() == 13)"
        "assert(ss.tellp() == 13)\n"
        // 9
        "ss.write('NOPQR')"
        "assert(ss.tellg() == 13)"
        "assert(ss.tellp() == 18)\n"
        // 10
        "buff, eof = ss.read(5)"
        "assert(buff == 'NOPQR')"
        "assert(not eof)"
        "assert(ss.tellg() == 18)"
        "assert(ss.tellp() == 18)\n"
        // 11
        "buff, eof = ss.read(5)"
        "assert(not buff)"
        "assert(not eof)"
        "assert(ss.tellg() == -1)"
        "assert(ss.tellp() == -1)\n"
    );
}

BOOST_AUTO_TEST_SUITE_END()