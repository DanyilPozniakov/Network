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
#include <queue>

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

    void AddConnection(const ConnectionInfo& connection);


    void ReadAndShowMessages();

protected:
    ISocket* serverSocket;
    WindowsServerSocket* windowsServerSocket;

    std::unique_ptr<std::thread> listener;
    std::unique_ptr<std::thread> receiver;
    std::unique_ptr<std::thread> messageReader;

private:
    std::atomic<bool> isRunning = false;
    std::vector<ConnectionInfo> connections;

    std::string host;
    std::string port;

    void Listener();
    void Receiver();
    std::string GetMassage();


};



#endif //CONNECTION_H
