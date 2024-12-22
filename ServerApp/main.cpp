#include <iostream>
#include <ServerSocket.h>
#include <ConnectInfo.h>

#include <ctime>


#include <iostream>


int main(int argc, char* argv[])
{

    ServerSocket serverSocket;
    auto info =  serverSocket.Listen();

    serverSocket.Receive();
    info.SetEndTime();
    auto time = info.GetStartTime();

    std::cout   << "---------------------------------\n"
                << "The connection is closed :\n"
                << "host:       " << info.GetHost() << ", port: " << info.GetPort() << "\n"
                << "Start time: " << info.GetStartTimeStr()
                << "end time:   " << info.GetEndTimeStr()
                << "duration:   " << info.GetDurationStr()
                << std::endl;

    return 0;
}
