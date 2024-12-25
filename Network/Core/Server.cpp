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
    serverSocket = new WindowsServerSocket(host, port);
    windowsServerSocket = dynamic_cast<WindowsServerSocket*>(serverSocket);
    if (!windowsServerSocket)
    {
        std::cerr << "Failed to cast serverSocket to WindowsServerSocket" << std::endl;
    }
}

Server::~Server()
{
    StopServer();
}


void Server::Run()
{
    if (windowsServerSocket->IsValid())
    {
        windowsServerSocket->Run();
    }
    //isRunning.store(true);


    while (true)
    {
        Massage message = windowsServerSocket->GetMassageFromQueue();
        if (message.message == "server stop")
        {
            windowsServerSocket->Stop();
            continue;
        }
        std::cout << "From: " << message.socketInfo.port << ",  Massage: " << message.message << std::endl;
    }
}

void Server::StopServer()
{
}

void Server::Restart()
{
    /**
     * @brief This function is used to restart the server.
     */
    StopServer();
    Run();
}
