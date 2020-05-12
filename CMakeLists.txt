cmake_minimum_required(VERSION 3.15)

include(cmake/DefineProjectOnce.cmake)

define_project_once(jive)

project(jive CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # This is the top-level project
    include(cmake/Standalone.cmake)
endif()

if(ENABLE_TESTING)
    enable_testing()
    message(
        "Building Tests. "
        "Be sure to check out test/constexpr_tests for constexpr testing")
    add_subdirectory(test)
endif()

add_subdirectory(jive)

option(ENABLE_UNITY "Enable Unity builds of projects" OFF)

if(ENABLE_UNITY)
  # Add for any project you want to apply unity builds for
  set_target_properties(jive PROPERTIES UNITY_BUILD ON)
endif()
