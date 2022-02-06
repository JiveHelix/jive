add_library(jive)

# Projects that include this project must #include "jive/<header-name>"
target_include_directories(jive PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(jive PUBLIC project_warnings project_options)

target_sources(
    jive
    PRIVATE
    numeric_string_compare.cpp
    time_value.cpp
    path.cpp
    list_directory.cpp
    format_paragraph.cpp)

install(TARGETS jive DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(
    DIRECTORY ${PROJECT_SOURCE_DIR}/jive
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
