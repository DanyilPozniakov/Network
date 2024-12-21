#include <iostream>
#include <ClientSocket.h>

#include <thread>

void rec(ClientSocket& serverSocket)
{
    serverSocket.Resave();
}

int main()
{
    ClientSocket serverSocket;
    serverSocket.ConnectToServer();

    std::thread t1(rec, std::ref(serverSocket));

    int count = 0;
    while(count < 10)
    {
        std::string request;
        std::getline(std::cin, request);
        serverSocket.Send(request);
        count++;
    }


    t1.join();
    return 0;
}

