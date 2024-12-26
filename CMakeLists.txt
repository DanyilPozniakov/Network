cmake_minimum_required(VERSION 3.29)
project(Networking)

set(CMAKE_CXX_STANDARD 23)

add_subdirectory(Network)
add_subdirectory(ServerApp)
add_subdirectory(Network/CLI)

add_executable(Networking main.cpp)

target_link_libraries(Networking PRIVATE Network)