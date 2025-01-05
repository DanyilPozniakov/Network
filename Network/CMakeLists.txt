
add_library(Network STATIC
        Sockets/WindowsServerTCPSocket.cpp
        Sockets/WindowsServerTCPSocket.h
        Sockets/WindowsClientSocket.cpp
        Sockets/WindowsClientSocket.h
        Core/Server.cpp
        Core/Server.h
        Core/ConnectionInfo.h
        Core/ConnectionInfo.cpp
        Sockets/ISocket.cpp
        Sockets/ISocket.h
        Core/CLI/CLI.cpp
        Core/CLI/CLI.h
        Core/Utils.h
        Core/CLI/Command.cpp
        Core/CLI/Command.h
        Core/CLI/CommandManager.cpp
        Core/CLI/CommandManager.h

)

target_include_directories(Network PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Sockets
        ${CMAKE_CURRENT_SOURCE_DIR}/Core

)

