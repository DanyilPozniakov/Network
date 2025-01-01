//
// Created by Pozniakov Danyil on 21.12.2024.
//

#include "Server.h"
#include <mutex>
#include <regex>
#include <CLI.h>

//temp
#include <iostream>


Server::Server(const std::string& host, const std::string& port)
{
    serverSocket = new WindowsServerSocket(host, port);
    windowsServerSocket = dynamic_cast<WindowsServerSocket*>(serverSocket);

    if (!windowsServerSocket)
    {
        std::cerr << "Failed to cast serverSocket to WindowsServerSocket" << std::endl;
    }

    SetSLICommands();
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
        if(!cli.ExecuteIfCommand(message.message))
        {
            windowsServerSocket->AddMessageToOutgoingQueue({"Unknown command! Enter 'help' to see all " \
                                                        "commands", message.socketInfo});
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

void Server::SendToAll(const std::string& message)
{
}

void Server::SetSLICommands()
{
    //Set up CLI flags
    //TODO: Add flags...


    // Set up CLI commands
    cli.AddCommand("command1", [this]()
    {
        std::cout << "Command base executed" << std::endl;
    });

    cli.AddCommand("login", [this](const std::string& args)
    {
        std::cout << "Login command executed\n";
        std::cout << args << std::endl;
    });
}
