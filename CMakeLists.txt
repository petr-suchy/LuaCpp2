cmake_minimum_required(VERSION 3.10)

project(LuaCpp
    VERSION 1.0.0
    DESCRIPTION ""
    HOMEPAGE_URL ""
    LANGUAGES CXX
)

include(GNUInstallDirs)
include(CTest)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(IS_LUACPP_TOPLEVEL_PROJECT TRUE)
else()
  set(IS_LUACPP_TOPLEVEL_PROJECT FALSE)
endif()

option(LUACPP_INSTALL_LIBRARY
    "Enable installing of LuaCpp library into default locations"
    ${IS_LUACPP_TOPLEVEL_PROJECT}
)
option(LUACPP_BUILD_TESTING
    "Build and run LuaCpp tests"
    ${IS_LUACPP_TOPLEVEL_PROJECT}
)
option(LUACPP_BUILD_EXAMPLE
    "Build LuaCpp example"
    ${IS_LUACPP_TOPLEVEL_PROJECT}
)

add_library(LuaCpp INTERFACE)
# add alias so the project can be used with add_subdirectory
add_library(LuaCpp::LuaCpp ALIAS LuaCpp)

target_include_directories(LuaCpp
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(LuaCpp INTERFACE cxx_std_14)

# add a dependency on the lua library
find_package(lua REQUIRED)
target_link_libraries(LuaCpp INTERFACE lua)

if(BUILD_TESTING AND LUACPP_BUILD_TESTING)
    add_subdirectory(tests)
endif()

if(LUACPP_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()

if(LUACPP_INSTALL_LIBRARY)

    # create a target set
    install(
        TARGETS LuaCpp
        EXPORT LuaCpp_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    )

    include(CMakePackageConfigHelpers)

    # create a version file
    write_basic_package_version_file(
        "LuaCppConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    # create a config file
    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/LuaCppConfig.cmake.in"
        "${PROJECT_BINARY_DIR}/LuaCppConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/luacpp/cmake
    )

    # install the target set
    install(
        EXPORT LuaCpp_Targets
        FILE LuaCppTargets.cmake
        NAMESPACE LuaCpp::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/luacpp/cmake
    )
    
    # install the config file and version file
    install(
        FILES
            "${PROJECT_BINARY_DIR}/LuaCppConfig.cmake"
            "${PROJECT_BINARY_DIR}/LuaCppConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/luacpp/cmake
    )
    
    # install LuaCpp headers
    install(
        DIRECTORY ${PROJECT_SOURCE_DIR}/include/luacpp
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

endif()