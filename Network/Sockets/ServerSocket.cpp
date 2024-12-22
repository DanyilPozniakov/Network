//
// Created by pozni on 21.12.2024.
//
#include <iostream>
#include "ServerSocket.h"

#include "ClientSocket.h"


ServerSocket::ServerSocket(const std::string& host, const std::string& port)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return;
    }

    addrinfo* result = nullptr;
    addrinfo hints{};

    //hints settings
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &result) != 0) //TODO: Add error handling
    {
        std::cout << "getaddrinfo failed: " << WSAGetLastError() << std::endl;
        return;
    }

    //Initialize the socket
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        return;
    }

    //Bind the socket in the system
    if (bind(ListenSocket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
    {
        std::cout << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        freeaddrinfo(result);
        return;
    }
    freeaddrinfo(result);

    //Listen for incoming connections
    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(ListenSocket);
        return;
    }

    std::cout << "ServerSocket created!" << std::endl;
}

ServerSocket::~ServerSocket()
{
    closesocket(ListenSocket);
    for (const auto& ClientSocket : ClientSockets)
    {
        closesocket(ClientSocket);
    }
    WSACleanup();
}


ConnectInfo ServerSocket::Listen()
{

        sockaddr_in clientInfo;
        int infoSize = sizeof(clientInfo);

        auto socketClient= accept(ListenSocket, reinterpret_cast<sockaddr*>(&clientInfo), &infoSize);
        if (socketClient == INVALID_SOCKET)
        {
            std::cout << "accept failed: " << WSAGetLastError() << std::endl;
            closesocket(ListenSocket);
            WSACleanup();
            return {};
        }
        else
        {
            ClientSockets.push_back(socketClient);
            char host[NI_MAXHOST];
            inet_ntop(AF_INET, &clientInfo.sin_addr,host, NI_MAXHOST);
            int port = ntohs(clientInfo.sin_port);

            std::string hostStr(host);
            std::string portStr = std::to_string(port);
            std::cout << "Client connected: " << host << " on port " << port << std::endl;

            ConnectInfo connectInfo(hostStr, portStr);
            connectInfo.SetIsConnected(true);
            return connectInfo;


        }

}

void ServerSocket::Send(const std::string& answer)
{
    send(ClientSockets[0], answer.c_str(), answer.size(), 0);
}


void ServerSocket::Receive()
{
    while (true)
    {
        int bytesrecv = recv(ClientSockets[0], recvbuf, sizeof(recvbuf), 0);
        if (bytesrecv > 0)
        {
            recvbuf[bytesrecv] = '\0';
            //TODO: Add signal new message received


            Send("Message received" + std::string(recvbuf));
            std::cout << "Massage received: " << recvbuf << std::endl;
        }
        else if (bytesrecv == 0)
        {
            std::cout << "connection closed" << std::endl;
            break;
        }
        else
        {
            std::cout << "recv failed: " << WSAGetLastError() << std::endl;
            break;
        }
    }
}


std::string ServerSocket::ReadBuffer()
{
    return std::string(recvbuf);
}
