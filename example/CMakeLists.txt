cmake_minimum_required(VERSION 3.10)

find_package(Lua)

if(NOT LUA_FOUND)
    message(WARNING "Lua not found, example won't be build")
    return()
endif()

add_executable(LuaCpp2Example)
target_sources(LuaCpp2Example
    PRIVATE
        src/LuaLibInst.cpp
        src/Main.cpp
        src/Geometry.h
        src/Geometry.cpp
)

target_include_directories(LuaCpp2Example
    PRIVATE
        ${LUA_INCLUDE_DIR}
)

target_link_libraries(LuaCpp2Example
    PRIVATE
        ${LUA_LIBRARIES}
        LuaCpp2::LuaCpp2
)