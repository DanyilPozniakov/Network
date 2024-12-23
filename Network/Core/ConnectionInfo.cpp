//
// Created by Pozniakov Danyil on 21.12.2024.
//

#include "ConnectionInfo.h"

#include <utility>

void ConnectionInfo::SetId(int id)
{
    this->id = id;
}

void ConnectionInfo::AddRequest()
{
    this->request_count++;
}

ConnectionInfo::ConnectionInfo(std::string&  host, int  port)
    : host(std::move(host)), port(port)
{
    auto time = std::time(nullptr);
    start_time = std::mktime(std::localtime(&time));
}

ConnectionInfo::ConnectionInfo(const char* host, int port)
{
    std::string hostStr(host);
    auto time = std::time(nullptr);
    start_time = std::mktime(std::localtime(&time));
}

bool ConnectionInfo::IsValid() const
{
    return port || host.empty();
}

bool ConnectionInfo::IsConnected() const
{
    return is_connected;
}

std::time_t ConnectionInfo::GetStartTime() const
{
    return start_time;
}

std::time_t ConnectionInfo::GetEndTime() const
{
    return end_time;
}

std::time_t ConnectionInfo::GetDuration() const
{
    return duration;
}

std::time_t ConnectionInfo::GetCurrentLocalTime() const
{
    auto time = std::time(nullptr);
    return std::mktime(std::localtime(&time));
}

std::string ConnectionInfo::GetStartTimeStr() const
{
    return std::ctime(&start_time);
}

std::string ConnectionInfo::GetEndTimeStr() const
{
    return std::ctime(&end_time);
}

std::string ConnectionInfo::GetDurationStr() const
{
    return std::to_string(std::difftime(end_time, start_time)) + "seconds";
}

void ConnectionInfo::SetDuration()
{
    this->duration = end_time - start_time;
}

void ConnectionInfo::SetName(const std::string& name)
{
    this->name = name;
}

void ConnectionInfo::SetEndTime()
{
    end_time = std::time(nullptr);
    SetDuration();
}

void ConnectionInfo::AddResponse(std::string& response)
{
    responses.push_back(response);
}

void ConnectionInfo::AddError(const std::string& error)
{
    errors.push_back(error);
}

void ConnectionInfo::AddRequest(const std::string& request)
{
    ++request_count;
    requests.push_back(request);
}

void ConnectionInfo::SetIsConnected(bool is_connected)
{
    this->is_connected = is_connected;
}

std::string ConnectionInfo::GetHost() const
{
    return host;
}

int ConnectionInfo::GetPort() const
{
    return port;
}

std::string ConnectionInfo::GetName() const
{
    return name;
}
