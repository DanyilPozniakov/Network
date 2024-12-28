//
// Created by Pozniakov Danyil on 21.12.2024.
//
#include <iostream>
#include "WindowsServerSocket.h"
#include <mutex>
#include "WindowsClientSocket.h"


#define YELLOW_OUTPUT(message) \
std::cout << "\033[0;33m" << message << "\033[0m" << std::endl;

#define GREEN_OUTPUT(message) \
std::cout << "\033[1;32m" << message << "\033[0m" << std::endl;

#define RED_OUTPUT(message) \
std::cout << "\033[4;31m" << message << "\033[0m" << std::endl;


WindowsServerSocket::WindowsServerSocket(const std::string& host, const std::string& port)
{
    ISocket::host = host;
    ISocket::port = port;

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

    WindowsServerSocket::InitializeSocket();
}

WindowsServerSocket::~WindowsServerSocket()
{
    ClosesSocket();
    WSACleanup();
}

void WindowsServerSocket::Run()
{
    isRunning.store(true);
    std::thread SocketIO(&WindowsServerSocket::RunSocketIO, this);
    SocketIO.detach();
    std::cout << "Threads finished" << std::endl;
}

void WindowsServerSocket::Stop()
{
    isRunning.store(false);
    std::cout << "Socket server stopped!" << std::endl;
}

void WindowsServerSocket::InitializeSocket()
{
    //Initialize the socket
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET)
    {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        return;
    }

    //Bind the socket in the system
    if (bind(listenSocket, result->ai_addr, result->ai_addrlen) == SOCKET_ERROR)
    {
        auto error = WSAGetLastError();
        if (error == WSAEADDRINUSE)
        {
            std::cerr << "Address already in use: " << error << std::endl;
            closesocket(listenSocket);
            Stop();
            return;
        }

        std::cout << ": " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        freeaddrinfo(result);
        return;
    }
    freeaddrinfo(result);

    //Listen for incoming connections
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cout << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        return;
    }

    std::cout << "WindowsServerSocket created!" << std::endl;
}


void WindowsServerSocket::ClosesSocket()
{
    closesocket(listenSocket);
    for (const auto& ClientSocket : clientSockets)
    {
        closesocket(ClientSocket.socket);
    }
    std::cout << "WindowsServerSocket closed!" << std::endl;
}


void WindowsServerSocket::RunSocketIO()
{
    //TODO: restructuring the method, unite the RunSocketIO and Listen methods into one MainLoop method

    while (isRunning.load())
    {
        FD_SET listenFds;
        FD_ZERO(&listenFds);
        FD_SET(listenSocket, &listenFds);
        timeval timeout{0, 100};
        int result = select(0, &listenFds, nullptr, nullptr, &timeout);
        //YELLOW_OUTPUT("Listening for incoming connections...")
        if (result == SOCKET_ERROR)
        {
            std::cerr << "Listen socket failed: " << WSAGetLastError() << std::endl;
            //TODO:: Add error handling, signal to the main thread -> stop the server
            continue;
        }

        if (FD_ISSET(listenSocket, &listenFds))
        {
            sockaddr_in clientInfo;
            int infoSize = sizeof(clientInfo);
            SOCKET ClientSocket = accept(listenSocket, reinterpret_cast<sockaddr*>(&clientInfo), &infoSize);
            if (ClientSocket == INVALID_SOCKET)
            {
                std::cerr << "accept failed: " << WSAGetLastError() << std::endl;
                continue;
                //TODO: Add error handling, signal to the main thread
            }
            //Creating and save a connection info
            char host[NI_MAXHOST];
            int port = ntohs(clientInfo.sin_port);
            inet_ntop(AF_INET, &clientInfo.sin_addr, host, NI_MAXHOST);

            clientSockets.push_back({ClientSocket, serial++, port, host});
            GREEN_OUTPUT("Client connected: " << host << " on port: " << port)
        }

        if (clientSockets.empty())
        {
            //std::cerr << "No clients connected, waiting for the client...." << std::endl;
            continue;
        }

        //IO Logic
        FD_SET readFds, writesFds;
        FD_ZERO(&readFds);
        FD_ZERO(&writesFds);

        for (const auto& socket : clientSockets)
        {
            FD_SET(socket.socket, &readFds);
            FD_SET(socket.socket, &writesFds);
        }

        int valid_sockets = select(0, &readFds, &writesFds, nullptr, nullptr);
        if (valid_sockets == SOCKET_ERROR) std::cerr << "Select failed: " << WSAGetLastError() << std::endl;

        for (auto socket_info = clientSockets.begin(); socket_info != clientSockets.end();)
        {
            if (FD_ISSET(socket_info->socket, &readFds))
            {
                int bytesrecv = recv(socket_info->socket, recvbuf.data(), recvbuf.size(), 0);
                if (bytesrecv > 0)
                {
                    recvbuf[bytesrecv] = '\0';
                    {
                        if (!clientSockets.empty())
                        {
                            {
                                std::lock_guard lock_message_queue(incoming_mtx);
                                incomingMessages.push({std::string(recvbuf.data()), *socket_info});
                            }
                            massageReceived_cv.notify_all();
                        }
                        else
                        {
                            std::cerr << "vector clientSockets is empty in RunSocketIO -> write" << std::endl;
                            ++socket_info;
                            continue;
                        }
                    }
                }
                else if (bytesrecv == 0)
                {
                    std::cerr << "Connection closed" << std::endl;
                    socket_info = clientSockets.erase(socket_info);
                    continue;
                }

                else if (bytesrecv == SOCKET_ERROR)
                {
                    std::cerr << "SOCKET_ERROR Recv failed: " << WSAGetLastError() << std::endl;
                    socket_info = clientSockets.erase(socket_info);
                    continue;
                }
                else
                {
                    std::cerr << "Recv failed: " << WSAGetLastError() << std::endl;
                    socket_info = clientSockets.erase(socket_info);
                    continue;
                }
            }

            if (outgoingMessages.empty())
            {
                ++socket_info;
                continue;
            }

            std::lock_guard lock_message_queue(outgoing_mtx);
            while (!outgoingMessages.empty())
            {
                if (FD_ISSET(socket_info->socket, &writesFds))
                {
                    Message message = outgoingMessages.front();
                    int bytes_send = send(message.socketInfo.socket, message.message.c_str(),
                                          message.message.size(), 0);
                    if (bytes_send == SOCKET_ERROR)
                    {
                        std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
                        socket_info = clientSockets.erase(socket_info);
                        continue;
                    }
                    outgoingMessages.pop();
                }
                else
                {
                    //TODO: Add error handling when the socket is not ready to write!
                    std::cerr << "Socket is not ready to write" << std::endl;
                }
            }
            ++socket_info;
        }
    }
}


bool WindowsServerSocket::IsValid()
{
    return listenSocket != INVALID_SOCKET;
}

Message WindowsServerSocket::GetMassageFromQueue()
{
    /*
     * This method blocks the thread when it is called until there is at least one message in the queue
     */

    std::unique_lock lock(incoming_mtx);
    massageReceived_cv.wait(lock, [this]()
    {
        if (clientSockets.empty())
        {
            RED_OUTPUT("Error in GetMassageFromQueue clientSockets empty")
            return false;
        }

        return !incomingMessages.empty();
    });
    Message massage = incomingMessages.front();
    incomingMessages.pop();
    return massage;
}

Message WindowsServerSocket::GetErrorFromQueue()
{
    std::lock_guard lock(errors_mtx);
    if (errors.empty()) return {};
    Message error = errors.front();
    return error;
}

void WindowsServerSocket::AddMassageToSendQueue(const Message& message)
{
    std::lock_guard lock(outgoing_mtx);
    outgoingMessages.push(message);
}
