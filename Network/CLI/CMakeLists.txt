add_library(cli STATIC
    CLI.cpp
    CLI.h)

target_include_directories(cli PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} )