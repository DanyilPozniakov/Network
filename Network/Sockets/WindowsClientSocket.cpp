//
// Created by pozni on 21.12.2024.
//

#include <iostream>
#include "WindowsClientSocket.h"

WindowsClientSocket::WindowsClientSocket(const std::string& host, const std::string& port)
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
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
    }


}

WindowsClientSocket::~WindowsClientSocket()
{
    WindowsClientSocket::Cleanup();
}

void WindowsClientSocket::ConnectToServer()
{
    if(connect(ConnectSocket,result->ai_addr,result->ai_addrlen) == SOCKET_ERROR)
    {
        std::cout << "Connect failed: " << WSAGetLastError() << std::endl;
        return;
    }
    else
    {
        std::cout << "Connected to server!" << std::endl;
    }

}

void WindowsClientSocket::Disconnect()
{
}

int WindowsClientSocket::Send(const std::string& request)
{
    int bytes_send = send(ConnectSocket, request.c_str(), request.size(),0);
    return bytes_send;
}

void WindowsClientSocket::Resave()
{
    //TODO: Add resaving logic...
    while(true)
    {
        ZeroMemory(recvbuf, 1024);
        int bytesReceived = recv(ConnectSocket, recvbuf, sizeof(recvbuf), 0);
        if(bytesReceived > 0)
        {
            recvbuf[bytesReceived] = '\0';
            std::string message = std::string(recvbuf, bytesReceived);
            std::cout << "Received: " << message << std::endl;

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

void WindowsClientSocket::Cleanup() const
{
    if(result != nullptr)
    {
        freeaddrinfo(result);
    }
    closesocket(ConnectSocket);
    WSACleanup();
}
