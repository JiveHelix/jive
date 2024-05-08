add_library(jive_catch_main STATIC catch_main.cpp)
target_link_libraries(jive_catch_main PUBLIC CONAN_PKG::catch2)
target_link_libraries(jive_catch_main PRIVATE project_options)

add_executable(
    jive_tests
    binary_io_tests.cpp
    buffer_tests.cpp
    circular_index_tests.cpp
    create_exception_tests.cpp
    format_tests.cpp
    id_bytes_tests.cpp
    multiply_rounded_tests.cpp
    overflow_tests.cpp
    path_tests.cpp
    power_tests.cpp
    precise_string_tests.cpp
    scope_flag_tests.cpp
    socket_tests.cpp
    strings_tests.cpp
    time_value_tests.cpp
    to_integer_tests.cpp
    to_float_tests.cpp
    comparison_operator_tests.cpp
    revision_tests.cpp)

target_link_libraries(
    jive_tests
    PRIVATE
    project_warnings
    project_options
    jive_catch_main
    jive)

add_version_header(jive_tests)

add_test(NAME JiveTests COMMAND ${CMAKE_BINARY_DIR}/bin/jive_tests)
