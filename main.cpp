#include <iostream>
#include <WindowsClientSocket.h>

#include <thread>

void rec(WindowsClientSocket& serverSocket)
{
    serverSocket.Resave();
}

int main()
{
    int* a = new int(1);
    {
        delete a;
    }
    int& b = *a;

    if(b == nullptr)
    {
        std::cout << "nullptr" << std::endl;
    }

    std::cout << b << std::endl;

    // WindowsClientSocket serverSocket;
    // serverSocket.ConnectToServer();
    //
    // std::thread t1(rec, std::ref(serverSocket));
    //
    // while(true)
    // {
    //     std::string request;
    //     std::getline(std::cin, request);
    //     serverSocket.Send(request);
    // }
    //
    //
    // t1.join();
    // return 0;
}

