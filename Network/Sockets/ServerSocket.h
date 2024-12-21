//
// Created by Danyil Pozniakov on 21.12.2024.
//

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "0808"
#define DEFAULT_HOST "localhost"

class ServerSocket {
public:
    explicit ServerSocket(const std::string& host = DEFAULT_HOST, const std::string& port = DEFAULT_PORT);
    virtual ~ServerSocket();

    virtual void Listen();
    virtual void Send(const std::string& answer);
    virtual std::string Receive();

private:
    SOCKET ListenSocket = INVALID_SOCKET;
    WSADATA wsaData;
    int iResult = 0;
    std::vector<SOCKET> ClientSockets = {};

    char recvbuf[1024];

};



#endif //SERVERSOCKET_H
