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
#include <ConnectionInfo.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "0808"
#define DEFAULT_HOST "localhost"

class ServerSocket {
    friend class Server;
public:
    explicit ServerSocket(const std::string& host = DEFAULT_HOST, const std::string& port = DEFAULT_PORT);
    virtual ~ServerSocket();

    void InitializeSocket();
    void Close();
    ConnectionInfo Listen();
    void Send(const std::string& answer);
    void Receive();

    std::string ReadBuffer();

    bool IsValid();

protected:
    std::vector<SOCKET> ClientSockets = {};

private:
    addrinfo* result = nullptr;
    SOCKET ListenSocket = INVALID_SOCKET;
    WSADATA wsaData;
    int iResult = 0;

    char recvbuf[1024];

};



#endif //SERVERSOCKET_H
