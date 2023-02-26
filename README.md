# LuaCpp2

Lua is a powerful, fast, lightweight, embeddable scripting language. This means that you can link the Lua library with your applications to incorporate Lua facilities into them. At the same time, a program that uses Lua can register new functions in the Lua environment, so that they can add facilities that cannot be written directly in Lua. LuaCpp2 is a header-only wrapper around the Lua library that can make using Lua in your C++ application very easy.

An illustrative example:
```cpp
#include <iostream>
#include <sstream>
#include <LuaCpp2/LuaCpp2.h>

struct UserRecord {
    int user_id;
    std::string user_name;
};

// make the structure transferable between lua and C++
defhost_struct(
    UserRecord,
        m.user_id,
        m.user_name
)

int main(int argc, char* argv[])
{
    try {

        Lua::Engine lua(
            // initialises a new lua state
            [](Lua::Lua lua)
            {
                // for print() function
                lua.openStdLibs();

                // create a global factory function
                lua.global("CreateUserRecord").in() << Lua::MakeFunc(
                    [](Lua::Args args, Lua::Lua lua)
                    {
                        int user_id;
                        std::string user_name;

                        args.in() >> user_id >> user_name;
                        args.out() << UserRecord{user_id, user_name};
                    }
                );

            }
        );

        std::istringstream iss(
            "local rec = CreateUserRecord(1, 'Alice')"
            "print(rec.user_id, rec.user_name)"
        );

        // execute the script
        lua.doStream(iss);

        UserRecord rec;
        // call our factory function
        lua.pcall("CreateUserRecord", 2, "Bob") >> rec;
        // call a lua function
        lua.pcall("print", rec.user_id, rec.user_name);

    }
    catch (std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
    }

    return 0;
}
```
