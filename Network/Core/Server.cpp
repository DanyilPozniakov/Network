//
// Created by Pozniakov Danyil on 21.12.2024.
//

#include "Server.h"
#include <mutex>


//temp
#include <iostream>

std::mutex connection_info_mtx;

Server::Server(const std::string& host, const std::string& port)
{
    serverSocket = new ServerSocket(host, port);
}

Server::~Server()
{
    StopServer();
}

void Server::Init()
{
    if(!serverSocket->IsValid())
    {
        serverSocket->InitializeSocket();
    }
}

void Server::Run()
{
    /**
     * @brief This function is used to run the server.
     */
    if(serverSocket->IsValid())
    {
        listener = std::thread([this]{ serverSocket->Listen(); });
    }

    receiver = std::thread([this]{ serverSocket->Receive(); });

    while(isRunning.load())
    {
        //TODO: Add conditional variable or signal
        std::string message = serverSocket->ReadBuffer();
    }
}

void Server::StopServer()
{
    /**
     * @brief This function is used to stop the server.
     */
    isRunning.store(false);
    listener.join();
    serverSocket->Close();
}

void Server::Restart()
{
    /**
     * @brief This function is used to restart the server.
     */
    StopServer();
    Run();
}

void Server::Listener()
{
    /**
     * @brief This function is used to listen to the server socket until the server is running.
     *
     * @param isRunning
     */
    while(isRunning.load())
    {
        auto connectInfo = serverSocket->Listen();
        AddConnection(connectInfo);
    }


}

void Server::AddConnection(const ConnectionInfo& connection)
{
    std::lock_guard lock(connection_info_mtx);
    connections.push_back(connection);
}


