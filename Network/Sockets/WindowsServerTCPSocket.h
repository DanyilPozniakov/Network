//
// Created by Danyil Pozniakov on 21.12.2024.
//

#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H

#include <array>
#include <condition_variable>
#include <string>
#include <vector>
#include <queue>
#include <atomic>
#include <memory>

#include <Utils.h>
#include <ISocket.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT "2525"
#define DEFAULT_HOST "localhost"


class WindowsServerTCPSocket : public ISocket
{
public:
    explicit WindowsServerTCPSocket(const std::string& host = DEFAULT_HOST, const std::string& port = DEFAULT_PORT);
    ~WindowsServerTCPSocket() override;


    void Run();
    void Stop();
    void RunSocketIO() override;
    void InitializeSocket() override;
    void ClosesSocket() override;

    bool IsValid();

    Message GetMassageFromQueue();
    Message GetErrorFromQueue();
    void AddMessageToOutgoingQueue(const Message& message);

protected:
    std::vector<ClientInfo> clientSockets;

    std::queue<Message> incomingMessages;
    std::queue<Message> outgoingMessages;
    std::vector<Message> errors;

    std::atomic<bool> isRunning = false;
    std::condition_variable massageReceived_cv;
    std::condition_variable outgoingMessage_cv;
    std::condition_variable error_cv;
    std::mutex incoming_mtx;
    std::mutex outgoing_mtx;
    std::mutex errors_mtx;

private:
    std::array<char, 1024> recvbuf{};
    int serial = 0;
};

#endif //SERVERSOCKET_H
