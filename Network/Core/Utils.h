//
// Created by pozni on 1/2/2025.
//

#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <winsock2.h>

struct ClientInfo
{
    SOCKET socket;
    int id;
    int port;
    std::string host;
};

struct Message
{
    std::string message;
    ClientInfo socketInfo;
};

#endif //UTILS_H
