//
// Created by Pozniakov Danyil on 21.12.2024.
//

#include "Server.h"
#include <mutex>
#include <CLI.h>

//temp
#include <iostream>
#include <unordered_map>


std::mutex connection_info_mtx;

Server::Server(const std::string& host, const std::string& port)
{

    serverSocket = new WindowsServerSocket(host, port);
    windowsServerSocket = dynamic_cast<WindowsServerSocket*>(serverSocket);
    if (!windowsServerSocket)
    {
        std::cerr << "Failed to cast serverSocket to WindowsServerSocket" << std::endl;
    }


    // Set up CLI commands
    cli.AddCommand("restart",   [this] { Restart(); });
    cli.AddCommand("stop",      [this] { StopServer(); });

    cli.AddCommand("command 1", [this]
    {
        Message message{"command 1 execute!!!!", lastMessage->socketInfo };
        windowsServerSocket->AddMassageToSendQueue(message);
    });

    cli.AddCommand("command 2", [this]
    {
        Message message{"command 2 execute!!!!", lastMessage->socketInfo };
        windowsServerSocket->AddMassageToSendQueue(message);
    });
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
    isRunning.store(true);


    while (isRunning.load())
    {
        Message message = windowsServerSocket->GetMassageFromQueue();
        lastMessage = &message;
        if(cli.IsSliCommand(message.message))
        {
            continue;
        }
        else
        {
            std::cout << "From: " << message.socketInfo.port << ",  Massage: " << message.message << std::endl;
            Message messageout{"server received: " + message.message, message.socketInfo};
            windowsServerSocket->AddMassageToSendQueue(messageout);
        }
    }
}

void Server::StopServer()
{
    isRunning.store(false);
    windowsServerSocket->Stop();
}

void Server::Restart()
{
    /**
     * @brief This function is used to restart the server.
     */
    StopServer();
    Run();
}
