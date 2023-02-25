#include <iostream>
#include <fstream>
#include "Geometry.h"

int main(int argc, char* argv[])
{
    try {

        Lua::Engine lua(
            // initialises a new lua state
            [](Lua::Lua lua)
            {
                // for print() function
                lua.openStdLibs();

                // create a factory function for rectangles
                lua.global("Rectangle").in() << Lua::MakeFunc(
                    [](Lua::Args args, Lua::Lua lua)
                    {
                        int x1, y1, x2, y2;
                        args.in() >> x1 >> y1 >> x2 >> y2;
                        args.out() << Geometry::Rectangle(x1, y1, x2, y2);
                    }
                );

            }
        );

        std::ifstream ifs("scripts/use_rectangle.lua");

        if (!ifs) {
            throw std::runtime_error("cannot open lua script file");
        }

        // execute the script
        lua.doStream(ifs);

    }
    catch (std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
    }

    return 0;
}