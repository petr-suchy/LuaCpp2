cmake_minimum_required(VERSION 3.10)

add_executable(LuaCpp2Example)
target_sources(LuaCpp2Example
    PRIVATE
        src/Main.cpp
        src/Geometry.h
        src/Geometry.cpp
)

target_link_libraries(LuaCpp2Example
    PRIVATE
        LuaCpp2::LuaCpp2
)