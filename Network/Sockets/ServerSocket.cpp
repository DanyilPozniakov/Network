//
// Created by Pozniakov Danyil on 21.12.2024.
//
#include <iostream>
#include "ServerSocket.h"
#include <mutex>
#include "ClientSocket.h"

std::mutex buff_mtx;
std::mutex init_mtx;

ServerSocket::ServerSocket(const std::string& host, const std::string& port)
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return;
    }

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

    InitializeSocket();
}

ServerSocket::~ServerSocket()
{
    ClosesSocket();
    WSACleanup();
}

void ServerSocket::InitializeSocket()
{
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


void ServerSocket::ClosesSocket()
{
    closesocket(ListenSocket);
    for (const auto& ClientSocket : clientSockets)
    {
        closesocket(ClientSocket.second);
    }
    std::cout << "ServerSocket closed!" << std::endl;
}

void ServerSocket::Listen()
{
    while (true)
    {
        FD_SET listenFds;
        FD_ZERO(&listenFds);
        FD_SET(ListenSocket, &listenFds);
        timeval timeout{1, 0};
        int result = select(0, &listenFds, nullptr, nullptr, &timeout);
        if (result == SOCKET_ERROR)
        {
            std::cerr << "Listen socket failed: " << WSAGetLastError() << std::endl;
            //TODO:: Add error handling, signal to the main thread -> stop the server
            return;
        }

        if (FD_ISSET(ListenSocket, &listenFds))
        {
            sockaddr_in clientInfo;
            int infoSize = sizeof(clientInfo);
            SOCKET ClientSocket = accept(ListenSocket, reinterpret_cast<sockaddr*>(&clientInfo), &infoSize);
            if (ClientSocket == INVALID_SOCKET)
            {
                std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
                //TODO: Add error handling, signal to the main thread
            }
            clientSockets[serial++] = ClientSocket;

            //Creating and save a connection info
            char host[NI_MAXHOST];
            int port = ntohs(clientInfo.sin_port);
            inet_ntop(AF_INET, &clientInfo.sin_addr, host, NI_MAXHOST);
            {
                ConnectionInfo connectInfo(host, port);
                std::lock_guard lock(init_mtx);
                connections.push_back(connectInfo);
                client_socket_initialized = static_cast<int>(connections.size());
                socket_init_cv.notify_all();
            }
        }
    }
}

void ServerSocket::Send(const std::string& answer)
{
    send(clientSockets[0], answer.c_str(), answer.size(), 0);
}


void ServerSocket::Receive()
{
    FD_SET readFds;
    FD_ZERO(&readFds);
    for (const auto& socket : clientSockets)
    {
        FD_SET(socket.second, &readFds);
    }

    timeval timeout{1, 0};
    int valid_sockets = select(0, &readFds, nullptr, nullptr, &timeout);
    if (valid_sockets == SOCKET_ERROR)
    {
        std::cerr << "select failed: " << WSAGetLastError() << std::endl;
        //TODO: Add error handling, signal to the main thread
    }


    for (auto &socket : clientSockets)
    {
        if(FD_ISSET(socket.second,&readFds))
        {
            int bytesrecv = recv(socket.second, recvbuf.data(), recvbuf.size(), 0);
            if(bytesrecv > 0)
            {
                std::lock_guard lock(buff_mtx);
                recvbuf[bytesrecv] = '\0';
                {
                    std::lock_guard lock(queue_mtx);
                    messages.emplace(recvbuf.data());
                }
                massageReceived_cv.notify_all();
            }
            if(bytesrecv == 0)
            {
                std::cerr << "connection closed" << std::endl;
                //TODO: Add error handling, signal to the main thread
                break;
            }
            else
            {
                std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
            }
        }
        else
        {
            //TODO: Add handling for the case when the socket is not ready to read
        }
    }

    // while (true)
    // {
    //     {
    //         std::lock_guard lock(buff_mtx);
    //         bytesrecv = recv(clientSockets[0], recvbuf.data(), recvbuf.size(), 0);
    //     }
    //
    //     if (bytesrecv > 0)
    //     {
    //         {
    //             std::lock_guard lock_buff(buff_mtx);
    //             recvbuf[bytesrecv] = '\0';
    //             {
    //                 std::lock_guard lock_queue(queue_mtx);
    //                 messages.emplace(recvbuf.data());
    //             }
    //         }
    //         massageReceived_cv.notify_all();
    //     }
    //     else if (bytesrecv == 0)
    //     {
    //         std::cout << "connection closed" << std::endl;
    //         break;
    //     }
    //     else
    //     {
    //         std::cout << "recv failed: " << WSAGetLastError() << std::endl;
    //         break;
    //     }
    // }

}


std::string ServerSocket::GetBufferData()
{
    std::lock_guard lock(buff_mtx);
    return {recvbuf.data()};
}


bool ServerSocket::IsValid()
{
    return ListenSocket != INVALID_SOCKET;
}
