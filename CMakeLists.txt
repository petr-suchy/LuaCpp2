cmake_minimum_required(VERSION 3.10)

project(LuaCpp2
    VERSION 1.0.0
    DESCRIPTION ""
    HOMEPAGE_URL ""
    LANGUAGES CXX
)

include(GNUInstallDirs)
include(CTest)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(IS_LUACPP2_TOPLEVEL_PROJECT TRUE)
else()
  set(IS_LUACPP2_TOPLEVEL_PROJECT FALSE)
endif()

option(LUACPP2_INSTALL_LIBRARY
    "Enable installing of LuaCpp2 library into default locations"
    ${IS_LUACPP2_TOPLEVEL_PROJECT}
)
option(LUACPP2_BUILD_TESTING
    "Build and run LuaCpp2 tests"
    ${IS_LUACPP2_TOPLEVEL_PROJECT}
)
option(LUACPP2_BUILD_EXAMPLE
    "Build LuaCpp2 example"
    ${IS_LUACPP2_TOPLEVEL_PROJECT}
)

add_library(LuaCpp2 INTERFACE)
# add alias so the project can be used with add_subdirectory
add_library(LuaCpp2::LuaCpp2 ALIAS LuaCpp2)

target_include_directories(LuaCpp2
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(LuaCpp2 INTERFACE cxx_std_14)

if(BUILD_TESTING AND LUACPP2_BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(LUACPP2_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

if(LUACPP2_INSTALL_LIBRARY)

    # create a target set
    install(
        TARGETS LuaCpp2
        EXPORT LuaCpp2_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    include(CMakePackageConfigHelpers)

    # create a version file
    write_basic_package_version_file(
        "LuaCpp2ConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    # create a config file
    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/LuaCpp2Config.cmake.in"
        "${PROJECT_BINARY_DIR}/LuaCpp2Config.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/LuaCpp2/cmake
    )

    # install the target set
    install(
        EXPORT LuaCpp2_Targets
        FILE LuaCpp2Targets.cmake
        NAMESPACE LuaCpp2::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/LuaCpp2/cmake
    )
    
    # install the config file and version file
    install(
        FILES
            "${PROJECT_BINARY_DIR}/LuaCpp2Config.cmake"
            "${PROJECT_BINARY_DIR}/LuaCpp2ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/LuaCpp2/cmake
    )
    
    # install LuaCpp2 headers
    install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/include/LuaCpp2
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

endif()