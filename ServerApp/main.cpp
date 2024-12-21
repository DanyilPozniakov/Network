#include <iostream>
#include <ServerSocket.h>


int main(int argc, char* argv[])
{
    ServerSocket serverSocket;
    serverSocket.Listen();

    serverSocket.Receive();

    return 0;
}
