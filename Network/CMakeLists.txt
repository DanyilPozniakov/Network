add_library(Network STATIC
        Sockets/ServerSocket.cpp
        Sockets/ServerSocket.h
        Sockets/ClientSocket.cpp
        Sockets/ClientSocket.h
        Core/Server.cpp
        Core/Server.h
        Core/Status.cpp
        Core/Status.h
        Core/ConnectionInfo.h
        Core/ConnectionInfo.cpp
        Core/ServerConfig.cpp
        Core/ServerConfig.h)

target_include_directories(Network PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Sockets
        ${CMAKE_CURRENT_SOURCE_DIR}/Core

)