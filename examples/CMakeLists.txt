add_executable(colorize colorize.cpp)

target_link_libraries(
    colorize
    PRIVATE
    project_warnings
    project_options
    jive)
