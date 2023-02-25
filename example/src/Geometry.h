#pragma once

#include <luacpp/luacpp.h>

namespace Geometry {

    struct Point {
        int x;
        int y;
    };

    struct Rectangle {

        Point leftTop;
        Point rightBottom;

        Lua::Function width;
        Lua::Function height;
        Lua::Function area;

        Rectangle(int x1, int y1, int x2, int y2);
        Rectangle();

    };

}

defhost_struct(
    Geometry::Point,
    m.x,
    m.y
)

defhost_struct(
    Geometry::Rectangle,
    m.leftTop,
    m.rightBottom,
    m.width,
    m.height,
    m.area
)





