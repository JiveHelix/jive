add_catch2_test(
    NAME jive_tests
    SOURCES
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
        thread_pool_tests.cpp
        time_value_tests.cpp
        to_integer_tests.cpp
        to_float_tests.cpp
        comparison_operator_tests.cpp
        revision_tests.cpp
    LINK jive)
