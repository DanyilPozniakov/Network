//
// Created by Pozniakov Danyil on 21.12.2024.
//

#ifndef CONNECTION_H
#define CONNECTION_H
#include <ConnectionInfo.h>
#include <ServerSocket.h>

//TODO: CLI!!!

class Server
{
public:
    ConnectionInfo connectInfo;

    explicit  Server(const std::string& host, const std::string& port);
    explicit  Server(const ConnectionInfo& connectInfo);
    ~Server();


protected:
    ServerSocket serverSocket;

private:


};



#endif //CONNECTION_H
