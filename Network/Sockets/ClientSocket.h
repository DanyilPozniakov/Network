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

#define DEFAULT_PORT "0808"
#define DEFAULT_HOST "localhost"

class ClientSocket {
public:
    explicit  ClientSocket(const std::string& host = DEFAULT_HOST, const std::string& port = DEFAULT_PORT);
    virtual ~ClientSocket();

    virtual void ConnectToServer();
    virtual int  Send(const std::string& request);
    virtual void Resave() ;
    virtual void Cleanup() const;


private:
    WSADATA wsaData;
    addrinfo  *result = nullptr;
    int iResult;

    SOCKET ConnectSocket = INVALID_SOCKET;

    char recvbuf[1024];


};



#endif //CLIENTSOCKET_H
