//
// Created by Pozniakov Danyil on 21.12.2024.
//

#ifndef CONNECTION_H
#define CONNECTION_H
#include <ISocket.h>
#include <ConnectionInfo.h>
#include <WindowsServerSocket.h>
#include <atomic>
#include <memory>
#include <CLI.h>

#ifdef _DEBUG

#endif


struct Session
{
    std::string name;
    std::string password;
   // Role role;
};

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
    void SendToAll(const std::string& message);

protected:
    std::atomic<bool> isRunning = false;

    void SetSLICommands();
private:
    ISocket* serverSocket;
    WindowsServerSocket* windowsServerSocket;
    CLI cli;

    Message lastMessage;

};

#endif //CONNECTION_H
