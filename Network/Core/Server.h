//
// Created by Pozniakov Danyil on 21.12.2024.
//

#ifndef CONNECTION_H
#define CONNECTION_H
#include <ConnectionInfo.h>
#include <ServerSocket.h>
#include <thread>
#include <atomic>

//TODO: CLI!!!

class Server
{
public:
    ConnectionInfo connectInfo;

    explicit  Server(const std::string& host, const std::string& port);
    //explicit  Server(const ServerConfig& config); //TODO: realize ServerConfig
    ~Server();

    void Init();
    void Run();
    void StopServer();
    void Restart();
    void Listener();

    void AddConnection(const ConnectionInfo& connection);

protected:
    ServerSocket* serverSocket;
    std::thread listener;
    std::thread receiver;

private:
    std::atomic<bool> isRunning = false;
    std::vector<ConnectionInfo> connections;

    std::string host;
    std::string port;
};



#endif //CONNECTION_H
