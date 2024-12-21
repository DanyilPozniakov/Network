add_library(Network STATIC
        Sockets/ServerSocket.cpp
        Sockets/ServerSocket.h
        Sockets/ClientInfo.cpp
        Sockets/ClientInfo.h
        Sockets/ClientSocket.cpp
        Sockets/ClientSocket.h)

target_include_directories(Network PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Sockets

)