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
#include <memory>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "0808"
#define DEFAULT_HOST "localhost"


struct SocketInfo
{
    SOCKET socket{};
    int id;
    int port;
    std::string host;
};

struct Massage
{
    std::string message;
    SocketInfo socketInfo;
};

class WindowsServerSocket : public ISocket
{
    friend class Server;

public:
    explicit WindowsServerSocket(const std::string& host = DEFAULT_HOST, const std::string& port = DEFAULT_PORT);
    ~WindowsServerSocket() override;


    void Run();
    void Stop();
    void Listen();
    void RunSocketIO() override;
    void InitializeSocket() override;
    void ClosesSocket() override;

    bool IsValid();

    Massage GetMassageFromQueue();
    Massage GetErrorFromQueue();
    void AddMassageToSendQueue(const Massage& message);

protected:
    std::queue<Massage>     incomingMessages;
    std::queue<Massage>     outgoingMessages;
    std::vector<Massage>    errors;

    std::vector<SocketInfo> clientSockets;

    std::atomic<bool> isRunning = false;
    std::condition_variable massageReceived_cv;
    std::condition_variable outgoingMessage_cv;
    std::condition_variable error_cv;
    std::condition_variable client_socket_init_cv;
    std::mutex incoming_mtx;
    std::mutex outgoing_mtx;
    std::mutex errors_mtx;
    std::mutex buff_mtx;
    std::mutex client_init_mtx;

private:
    std::string port;
    std::string host;

    addrinfo*   result = nullptr;
    SOCKET      listenSocket = INVALID_SOCKET;
    WSADATA     wsaData{};

    std::array<char, 1024> recvbuf{};
    int serial = 0;
};


#endif //SERVERSOCKET_H
