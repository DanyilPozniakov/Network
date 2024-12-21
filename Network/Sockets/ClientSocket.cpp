//
// Created by pozni on 21.12.2024.
//

#include <iostream>
#include "ClientSocket.h"

ClientSocket::ClientSocket(const std::string& host, const std::string& port)
{
    addrinfo hints{};
    if(WSAStartup(MAKEWORD(2,2), &wsaData) != 0) //TODO: Add error handling
    {
        Cleanup();
        std::cout << "WSAStartup failed: " << WSAGetLastError() << std::endl;
    }

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if(getaddrinfo(host.c_str(), port.c_str(), &hints, &result) != 0)
    {
        Cleanup();
        std::cout << "getaddrinfo failed: " << WSAGetLastError() << std::endl;
    }

    ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if(ConnectSocket == INVALID_SOCKET)
    {
        Cleanup();
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
    }


}

ClientSocket::~ClientSocket()
{
    ClientSocket::Cleanup();
}

void ClientSocket::ConnectToServer()
{
    if(connect(ConnectSocket,result->ai_addr,result->ai_addrlen) == SOCKET_ERROR)
    {
        Cleanup();
        std::cout << "Connect failed: " << WSAGetLastError() << std::endl;
        return;
    }
    else
    {
        std::cout << "Connected to server!" << std::endl;
    }

}

int ClientSocket::Send(const std::string& request)
{
    int bytes_send = send(ConnectSocket, request.c_str(), request.size(),0);
    return bytes_send;
}

void ClientSocket::Resave()
{
    //TODO: Add resaving logic...
    while(true)
    {
        int bytesReceived = recv(ConnectSocket, recvbuf, 1024, 0);
        if(bytesReceived > 0)
        {
            recvbuf[bytesReceived] = '\0';
            std::cout << "Received: " << recvbuf << std::endl;

        }
        else if(bytesReceived == 0)
        {
            std::cout << "Connection closed" << std::endl;
            break;
        }
        else
        {
            std::cout << "recv failed: " << WSAGetLastError() << std::endl;
            break;
        }
    }

}

void ClientSocket::Cleanup() const
{
    if(result != nullptr)
    {
        freeaddrinfo(result);
    }
    closesocket(ConnectSocket);
    WSACleanup();
}
