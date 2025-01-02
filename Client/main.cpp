#include <iostream>
#include <WindowsClientSocket.h>

#include <thread>

void rec(WindowsClientSocket& serverSocket)
{
    serverSocket.Resave();
}

int main()
{
    WindowsClientSocket serverSocket;
    serverSocket.ConnectToServer();
    std::thread t1(rec, std::ref(serverSocket));

    //TODO: add exit command...
    while(true)
    {
        std::string request;
        std::getline(std::cin, request);
        serverSocket.Send(request);
    }

    t1.join();
    return 0;
}

