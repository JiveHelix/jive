add_library(jive)

if (${fPIC})
    set_property(TARGET jive PROPERTY POSITION_INDEPENDENT_CODE ON)
endif ()

find_package(Fmt REQUIRED)

# Projects that include this project must #include "jive/<header-name>"
target_include_directories(jive PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(
    jive
    PUBLIC
    project_warnings
    project_options
    fmt::fmt
    pthread)

target_sources(
    jive
    PRIVATE
    colorize.cpp
    format_paragraph.cpp
    huffman.cpp
    numeric_string_compare.cpp
    path.cpp
    thread_pool.cpp
    thread_priority.cpp
    time_value.cpp)

install(TARGETS jive DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/jive
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
