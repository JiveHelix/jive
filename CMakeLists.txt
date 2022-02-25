cmake_minimum_required(VERSION 3.15)

project(jive CXX)

include(${CMAKE_CURRENT_LIST_DIR}/cmake_includes/setup_project.cmake)
setup_project()

add_subdirectory(jive)

if (${PROJECT_IS_TOP_LEVEL})
    if (ENABLE_TESTING)
        add_subdirectory(test)
    endif ()
elseif (${RECURSIVE_BUILD_TESTS} AND ${ENABLE_TESTING})
    add_subdirectory(test)
endif ()
