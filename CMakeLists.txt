cmake_minimum_required(VERSION 3.15)

# detect whether jive has been added as a subproject
if(NOT DEFINED PROJECT_NAME)
    set(JIVE_NOT_SUBPROJECT ON)
endif()

include(cmake/DefineProjectOnce.cmake)

define_project_once(jive)

project(jive CXX)

if(JIVE_NOT_SUBPROJECT)
    # This is the top-level project
    include(cmake/Standalone.cmake)
    option(ENABLE_UNITY "Enable Unity builds of projects" OFF)
endif()

add_subdirectory(jive)


if (JIVE_NOT_SUBPROJECT)



    if(ENABLE_TESTING)
        enable_testing()
        add_subdirectory(test)
    endif()

elseif(RECURSIVE_BUILD_TESTS AND ENABLE_TESTING)

    # A parent project has requested tests.
    add_subdirectory(test)

endif()

if(ENABLE_UNITY)
  # Add for any project you want to apply unity builds for
  set_target_properties(jive PROPERTIES UNITY_BUILD ON)
endif()
