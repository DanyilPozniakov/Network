//
// Created by Pozniakov Danyil on 21.12.2024.
//

#ifndef CONNECTION_H
#define CONNECTION_H
#include <ISocket.h>
#include <ConnectionInfo.h>
#include <WindowsServerSocket.h>
#include <thread>
#include <atomic>
#include <memory>
#include <CLI.h>

#ifdef _DEBUG

#endif

//TODO: CLI!!!

class Server
{
public:
    ConnectionInfo connectInfo;

    explicit  Server(const std::string& host, const std::string& port);
    //explicit  Server(const ServerConfig& config);
    ~Server();
    void Run();
    void StopServer();
    void Restart();


protected:
    std::atomic<bool> isRunning = false;

    void SetSLICommands();
private:
    ISocket* serverSocket;
    WindowsServerSocket* windowsServerSocket;
    CLI cli;

    std::string host;
    std::string port;

    Message* lastMessage;

};

#endif //CONNECTION_H
