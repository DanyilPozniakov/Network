
add_library(Network STATIC
        Sockets/WindowsServerSocket.cpp
        Sockets/WindowsServerSocket.h
        Sockets/WindowsClientSocket.cpp
        Sockets/WindowsClientSocket.h
        Core/Server.cpp
        Core/Server.h
        Core/ConnectionInfo.h
        Core/ConnectionInfo.cpp
        Sockets/ISocket.cpp
        Sockets/ISocket.h
        Sockets/LinuxServeSocket.cpp
        Sockets/LinuxServeSocket.h
        Core/CLI.cpp
        Core/CLI.h
        Core/Utils.h

)

target_include_directories(Network PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Sockets
        ${CMAKE_CURRENT_SOURCE_DIR}/Core

)

