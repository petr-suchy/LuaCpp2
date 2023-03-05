#include <boost/test/unit_test.hpp>
#include "../VsTestExplorer.h"

#include <LuaCpp2/Luaux.h>

BOOST_AUTO_TEST_SUITE(Luaux__HostStreams)

BOOST_AUTO_TEST_CASE(testSeekableStream)
{
    Lua::Engine lua(
        [](Lua::Lua lua)
        {
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

    lua.doString("ss = StringStream('ABCDEFGHIJKLM')");

    Luaux::Engine::SeekableStreamIfce ssi;
    lua.global("ss").out() >> ssi;

    Luaux::Host::SeekableStream<char, size_t, size_t> ss(ssi);

    BOOST_TEST(ss.tellg() == 0);
    BOOST_TEST(ss.tellp() == 0);

    ss.seekg(0, ss.end);
    ss.seekp(0, ss.end);
    BOOST_TEST(ss.tellg() == 13);
    BOOST_TEST(ss.tellp() == 13);

    ss.seekg(0, ss.beg);
    ss.seekp(0, ss.beg);
    BOOST_TEST(ss.tellg() == 0);
    BOOST_TEST(ss.tellp() == 0);

    char buff[128];
    ss.read(buff, 5);
    BOOST_TEST(ss.gcount() == 5);
    BOOST_TEST((std::string{buff, 5} == "ABCDE"));
    BOOST_TEST(!ss.eof());
    BOOST_TEST(ss.tellg() == 5);
    BOOST_TEST(ss.tellp() == 0);

    ss.read(buff, 5);
    BOOST_TEST(ss.gcount() == 5);
    BOOST_TEST((std::string{buff, 5} == "FGHIJ"));
    BOOST_TEST(!ss.eof());
    BOOST_TEST(ss.tellg() == 10);
    BOOST_TEST(ss.tellp() == 0);

    ss.read(buff, 5);
    BOOST_TEST(ss.gcount() == 3);
    BOOST_TEST((std::string{buff, 3} == "KLM"));
    BOOST_TEST(ss.eof());
    BOOST_TEST(ss.tellg() == -1);
    BOOST_TEST(ss.tellp() == -1);

    ss.clear();
    BOOST_TEST(ss.tellg() == 13);
    BOOST_TEST(ss.tellp() == 0);

    ss.seekp(0, ss.end);
    BOOST_TEST(ss.tellg() == 13);
    BOOST_TEST(ss.tellp() == 13);

    ss.write("NOPQR", 5);
    BOOST_TEST(ss.tellg() == 13);
    BOOST_TEST(ss.tellp() == 18);

    ss.read(buff, 5);
    BOOST_TEST(ss.gcount() == 5);
    BOOST_TEST((std::string{buff, 5} == "NOPQR"));
    BOOST_TEST(!ss.eof());
    BOOST_TEST(ss.tellg() == 18);
    BOOST_TEST(ss.tellp() == 18);

    ss.read(buff, 5);
    BOOST_TEST(ss.gcount() == 0);
    BOOST_TEST(ss.eof());
    BOOST_TEST(ss.tellg() == -1);
    BOOST_TEST(ss.tellp() == -1);
}

BOOST_AUTO_TEST_SUITE_END()