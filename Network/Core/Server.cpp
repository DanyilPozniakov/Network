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

void Server::Init()
{
    if(!windowsServerSocket->IsValid())
    {
        serverSocket->InitializeSocket();
    }
}

void Server::Run()
{
    /**
     * @brief This function is used to run the server.
     */
    if(windowsServerSocket->IsValid())
    {
        listener        = std::make_unique<std::thread>(&Server::Listener, this);
        receiver        = std::make_unique<std::thread>(&Server::Receiver, this);
        isRunning.store(true);
        //messageReader   = std::make_unique<std::thread>(&Server::ReadAndShowMessages, this);
    }

    while(isRunning.load())
    {
        std::string message = GetMassage();
        std::cout << message << std::endl;
    }


}

void Server::StopServer()
{
    /**
     * @brief This function is used to stop the server.
     */
    isRunning.store(false, std::memory_order_release);
    listener->join();
    serverSocket->ClosesSocket();
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
        windowsServerSocket->Listen();
        // auto connectInfo = serverSocket->Listen();
        // AddConnection(connectInfo);
    }
}

void Server::Receiver()
{
    serverSocket->Receive();
    std::cerr << "Receiver stopped" << std::endl;
}

std::string Server::GetMassage()
{
    std::unique_lock queue_lock(windowsServerSocket->queue_mtx);
    windowsServerSocket->massageReceived_cv.wait(queue_lock,[this]() { return !windowsServerSocket->messages.empty(); });
    auto mess = windowsServerSocket->messages.front();
    if(!mess.empty())
    {
        windowsServerSocket->messages.pop();
        return mess;
    }
    return "";

}



void Server::AddConnection(const ConnectionInfo& connection)
{
    std::lock_guard lock(connection_info_mtx);
    connections.push_back(connection);
}

void Server::ReadAndShowMessages()
{
    std::cout << "ReadAndShowMessages" << std::endl;
    while(isRunning.load())
    {
        std::string message = GetMassage();
        std::cout << message << std::endl;
    }
    std::cerr << "ReadAndShowMessages stopped" << std::endl;
}


