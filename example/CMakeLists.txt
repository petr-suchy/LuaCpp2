cmake_minimum_required(VERSION 3.10)

add_executable(luacpp_example)
target_sources(luacpp_example
    PRIVATE
        src/Main.cpp
        src/Geometry.h
        src/Geometry.cpp
)

target_link_libraries(luacpp_example
    PRIVATE
        LuaCpp::LuaCpp
)