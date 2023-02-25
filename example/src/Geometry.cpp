#include "Geometry.h"

namespace Geometry {

    Rectangle::Rectangle(int x1, int y1, int x2, int y2) :

        leftTop{ x1, y1 },
        rightBottom{ x2, y2 },

        width(
            Lua::MakeFunc(
                [](Lua::Args args, Lua::Lua lua)
                {
                    Rectangle rect;
                    args.in() >> rect;
                    args.out() << (rect.rightBottom.x - rect.leftTop.x);
                }
            )
        ),

        height(
            Lua::MakeFunc(
                [](Lua::Args args, Lua::Lua lua)
                {
                    Rectangle rect;
                    args.in() >> rect;
                    args.out() << (rect.rightBottom.y - rect.leftTop.y);
                }
            )
        ),

        area(
            Lua::MakeFunc(
                [](Lua::Args args, Lua::Lua lua)
                {
                    Rectangle rect;
                    args.in() >> rect;

                    float width, height;

                    // call member functions to get the width and height
                    lua.pcall(rect.width, rect) >> width;
                    lua.pcall(rect.height, rect) >> height;

                    args.out() << (width * height);
                }
            )
        )

    {}

    Rectangle::Rectangle() :
        Rectangle(0, 0, 0, 0)
    {}

}