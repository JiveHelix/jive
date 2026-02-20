add_executable(colorize colorize.cpp)

target_link_libraries(
    colorize
    PRIVATE
    project_warnings
    project_options
    jive)


add_executable(version version.cpp)

target_link_libraries(
    version
    PRIVATE
    project_warnings
    project_options
    jive)


# MSVC only
if (MSVC)
    add_executable(cpuinfo cpuinfo.cpp)
endif ()
