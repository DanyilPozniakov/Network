add_library(Network STATIC
        Sockets/WindowsServerSocket.cpp
        Sockets/WindowsServerSocket.h
        Sockets/WindowsClientSocket.cpp
        Sockets/WindowsClientSocket.h
        Core/Server.cpp
        Core/Server.h
        Core/Status.cpp
        Core/Status.h
        Core/ConnectionInfo.h
        Core/ConnectionInfo.cpp
        Core/ServerConfig.cpp
        Core/ServerConfig.h
        Sockets/ISocket.cpp
        Sockets/ISocket.h
        Sockets/LinuxServeSocket.cpp
        Sockets/LinuxServeSocket.h
        CLI/CLI.cpp
        CLI/CLI.h)

target_include_directories(Network PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Sockets
        ${CMAKE_CURRENT_SOURCE_DIR}/Core

)