cmake_minimum_required(VERSION 3.10)

include(BoostTestHelpers.cmake)

find_package(lua)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(
  luacpp_test_sources
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

add_executable(luacpp_tests)

target_sources(luacpp_tests PRIVATE ${luacpp_test_sources})
target_compile_features(luacpp_tests PRIVATE cxx_std_17)
target_link_libraries(luacpp_tests lua Boost::unit_test_framework)

boost_discover_tests(luacpp_tests ${luacpp_test_sources})
