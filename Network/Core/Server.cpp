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
    if(!windowsServerSocket->IsValid())
    {
        std::cerr << "Server is not valid!" << std::endl;
        return;
    }

    windowsServerSocket->Run();
    isRunning.store(true);

    while (isRunning.load())
    {
        Message message = windowsServerSocket->GetMassageFromQueue();
        lastMessage = message;
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

    cli.AddCommand("help", [this]()
    {
        std::string helpMessage = R"(Available commands:
    login -u <username> -p <password> - login to the server
    help        -   show all available commands
    command1    -  command1 description)";

        windowsServerSocket->AddMessageToOutgoingQueue({helpMessage, lastMessage.socketInfo});


    });



    cli.AddCommand("login", [this](const std::string& args)
    {

        std::smatch match;
        std::regex reg(R"(login\s-u\s(\w+)\s-p\s(\w+))");
        std::regex_match(args, match, reg);
        for(auto& m : match)
        {
            std::cout << m << std::endl;
        }
        std::cout << "Login command executed\n";
        windowsServerSocket->AddMessageToOutgoingQueue({"Login to the server", lastMessage.socketInfo});
    });

}
