add_executable(cli
        main_slitest.cpp
        CLI.cpp
        CLI.h
)

target_include_directories(cli PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR} )