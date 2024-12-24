//
// Created by Danyil Pozniakov on 21.12.2024.
//

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <array>
#include <condition_variable>
#include <string>
#include <vector>
#include <ConnectionInfo.h>
#include <queue>
#include <ISocket.h>
#include <atomic>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "0808"
#define DEFAULT_HOST "localhost"


struct SocketInfo
{
    std::queue<std::string>     incomingMessages;
    std::queue<std::string>     outgoingMessages;
    SOCKET socket{};
    int id{};
};

class WindowsServerSocket : public ISocket {
    friend class Server;
public:
    explicit WindowsServerSocket(const std::string& host = DEFAULT_HOST, const std::string& port = DEFAULT_PORT);
    ~WindowsServerSocket() override;

    void Send(const std::string& answer, int id) override;
    void RunSocketIO() override;
    void InitializeSocket() override;
    void ClosesSocket() override;
    void Listen();


    std::string GetBufferData();

    bool IsValid();

protected:
    std::queue<std::string>     errors;

    std::vector<SocketInfo>     clientSockets;
    std::vector<ConnectionInfo> connections;


    std::atomic<bool> isRunning = false;
    std::condition_variable massageReceived_cv;
    std::condition_variable client_socket_init_cv;
    std::mutex queue_mtx;


private:
    addrinfo* result = nullptr;
    SOCKET ListenSocket = INVALID_SOCKET;
    WSADATA wsaData{};

    std::array<char, 1024> recvbuf{};
    int serial = 0;

};



#endif //SERVERSOCKET_H
