//
// Created by Danyil Pozniakov on 21.12.2024.
//

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <array>
#include <condition_variable>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <string>
#include <vector>
#include <ConnectionInfo.h>
#include <map>
#include <queue>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "0808"
#define DEFAULT_HOST "localhost"




class ServerSocket {
    friend class Server;
public:
    explicit ServerSocket(const std::string& host = DEFAULT_HOST, const std::string& port = DEFAULT_PORT);
    virtual ~ServerSocket();

    void InitializeSocket();
    void ClosesSocket();
    void Listen();
    void Send(const std::string& answer);
    void Receive();

    std::string GetBufferData();

    bool IsValid();

protected:
    std::map<int,SOCKET>        clientSockets = {};
    std::queue<std::string>     messages;
    std::vector<ConnectionInfo> connections;

    std::condition_variable massageReceived_cv;
    std::condition_variable socket_valid_cv;
    std::condition_variable socket_init_cv;
    std::mutex queue_mtx;


private:
    addrinfo* result = nullptr;
    SOCKET ListenSocket = INVALID_SOCKET;
    WSADATA wsaData{};
    FD_SET readFds;
    FD_SET writeFds;

    int client_socket_initialized = 0;

    std::array<char, 1024> recvbuf{};

    int serial = 0;

};



#endif //SERVERSOCKET_H
