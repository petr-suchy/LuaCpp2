cmake_minimum_required(VERSION 3.10)

include(BoostTestHelpers.cmake)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

if(NOT TARGET Boost::unit_test_framework)
    message(WARNING "Boost::unit_test_framework not found, not building tests")
    return()
endif()

add_executable(luacpp_tests)
target_sources(luacpp_tests
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

target_link_libraries(luacpp_tests
    PRIVATE
        LuaCpp::LuaCpp
        Boost::unit_test_framework
)

boost_discover_tests(luacpp_tests)