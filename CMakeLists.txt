cmake_minimum_required(VERSION 3.10)

project(
    LuaCpp
        LANGUAGES CXX
)

include(CTest)

#################################################

add_library(LuaCpp INTERFACE)
add_library(LuaCpp::LuaCpp ALIAS LuaCpp)

target_include_directories(
    LuaCpp
        INTERFACE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(LuaCpp INTERFACE cxx_std_14)

#################################################

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
