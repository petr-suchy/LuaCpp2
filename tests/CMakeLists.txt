cmake_minimum_required(VERSION 3.10)

include(BoostTestHelpers.cmake)

find_package(Boost COMPONENTS unit_test_framework)

if(NOT TARGET Boost::unit_test_framework)
    message(WARNING "Boost::unit_test_framework not found, tests won't be build")
    return()
endif()

find_package(Lua)

if(NOT LUA_FOUND)
    message(WARNING "Lua not found, tests won't be build")
    return()
endif()

add_executable(LuaCpp2Tests)
target_sources(LuaCpp2Tests
    PRIVATE
        src/Main.cpp
        src/State/ClosureTests.cpp
        src/State/FunctionTests.cpp
        src/State/ChunkTests.cpp
        src/State/NumberTests.cpp
        src/State/PointerTests.cpp
        src/State/ReferenceTests.cpp
        src/State/StackSlotTests.cpp
        src/State/StringTests.cpp
        src/State/TableTests.cpp
        src/State/UserDataTests.cpp
        src/Transfer/TransferTests.cpp
        src/ValueCopy/ValueCopyTests.cpp
        src/Parameters/ParameterTests.cpp
        src/Engine/EngineTests.cpp    
)

target_include_directories(LuaCpp2Tests
    PRIVATE
        ${LUA_INCLUDE_DIR}
)

target_link_libraries(LuaCpp2Tests
    PRIVATE
        ${LUA_LIBRARIES}
        LuaCpp2::LuaCpp2
        Boost::unit_test_framework
)

boost_discover_tests(LuaCpp2Tests)