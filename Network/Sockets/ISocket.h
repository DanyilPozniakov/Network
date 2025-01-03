//
// Created by pozni on 12/24/2024.
//

#ifndef ISOCKET_H
#define ISOCKET_H

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string>

class ISocket {
public:
    ISocket() = default;
    virtual ~ISocket() = default;

    virtual void RunSocketIO() = 0;
    virtual void InitializeSocket() = 0;
    virtual void ClosesSocket() = 0;

protected:
    std::string port;
    std::string host;

    addrinfo*   result = nullptr;
    SOCKET      listenSocket = INVALID_SOCKET;
    WSADATA     wsaData{};

};



#endif //ISOCKET_H
