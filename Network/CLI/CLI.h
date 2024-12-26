
#ifndef CLI_H
#define CLI_H

#include <string>
#include <functional>
#include <map>

class CLI {
public:
    CLI() = default;
    ~CLI() = default;


    void execute(std::string command);
    void add_command(const std::string& command, std::function<void()> func);
    std::map<std::string, std::function<void()>> commands;



};



#endif //CLI_H
