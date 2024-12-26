//
// Created by Pozniakov Danyil on 21.12.2024.
//

#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "2525"
#define DEFAULT_HOST "localhost"

class WindowsClientSocket {
public:
    explicit  WindowsClientSocket(const std::string& host = DEFAULT_HOST, const std::string& port = DEFAULT_PORT);
    virtual ~WindowsClientSocket();

    void ConnectToServer();
    void Disconnect();
    int  Send(const std::string& request);
    void Resave();
    void Cleanup() const;






private:
    WSADATA wsaData;
    addrinfo  *result = nullptr;
    int iResult;

    SOCKET ConnectSocket = INVALID_SOCKET;

    char recvbuf[1024];


};



#endif //CLIENTSOCKET_H
