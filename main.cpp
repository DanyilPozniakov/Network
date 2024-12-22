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

    while(true)
    {
        std::string request;
        std::getline(std::cin, request);
        serverSocket.Send(request);
    }


    t1.join();
    return 0;
}

