
#ifndef CLIENTINFO_H
#define CLIENTINFO_H

#include <string>
#include <vector>
#include <ctime>

class ConnectionInfo {
    friend class Server;

public:
    ConnectionInfo() = default;
    ConnectionInfo(std::string&  host, std::string&  port);

    [[nodiscard]] bool IsValid() const;
    [[nodiscard]] bool IsConnected() const;
    void SetIsConnected(bool is_connected);

    [[nodiscard]] std::string GetHost() const;
    [[nodiscard]] std::string GetPort() const;
    [[nodiscard]] std::string GetName() const;
    [[nodiscard]] std::time_t GetStartTime() const;
    [[nodiscard]] std::time_t GetEndTime() const;
    [[nodiscard]] std::time_t GetDuration() const;
    [[nodiscard]] std::string GetStartTimeStr() const;
    [[nodiscard]] std::string GetEndTimeStr() const;
    [[nodiscard]] std::string GetDurationStr() const;

protected:
    void SetId(int id);
    void AddRequest();
    void SetDuration();
    void SetName(const std::string& name);
    void SetEndTime();

    void AddResponse(std::string& response);
    void AddError(const std::string& error);
    void AddRequest(const std::string& request);

private:
    std::string host;
    std::string port;
    std::string name;
    std::time_t start_time = 0;
    std::time_t end_time = 0;
    std::time_t duration = 0;

    int id = 0;
    int request_count = 0;
    int response_count = 0;
    int error_count = 0;

    bool is_connected = false;

    std::vector<std::string> errors;
    std::vector<std::string> requests;
    std::vector<std::string> responses;

};



#endif //CLIENTINFO_H