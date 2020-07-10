# automatically enable catch2 to generate ctest targets
if(CONAN_CATCH2_ROOT_DEBUG)
  include(${CONAN_CATCH2_ROOT_DEBUG}/lib/cmake/Catch2/Catch.cmake)
else()
  include(${CONAN_CATCH2_ROOT}/lib/cmake/Catch2/Catch.cmake)
endif()

add_library(jive_catch_main STATIC catch_main.cpp)
target_link_libraries(jive_catch_main PUBLIC CONAN_PKG::catch2)
target_link_libraries(jive_catch_main PRIVATE project_options)

add_executable(
    jive_tests
    angles_tests.cpp
    binary_io_tests.cpp
    buffer_tests.cpp
    create_exception_tests.cpp
    id_bytes_tests.cpp
    list_directory_tests.cpp
    multiply_rounded_tests.cpp
    path_tests.cpp
    power_tests.cpp
    strings_tests.cpp
    time_value_tests.cpp
    to_integer_tests.cpp
    to_float_tests.cpp)

target_link_libraries(
    jive_tests
    PRIVATE
    project_warnings
    project_options
    jive_catch_main
    jive)

# automatically discover tests that are defined in catch based test files you
# can modify the unittests. TEST_PREFIX to whatever you want, or use different
# for different binaries
catch_discover_tests(
    jive_tests
    TEST_PREFIX
    "unittests."
    EXTRA_ARGS)
