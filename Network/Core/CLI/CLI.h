#ifndef CLI_H
#define CLI_H

#include <string>
#include <functional>
#include <map>
#include <regex>




class CLI
{
public:
    CLI();
    ~CLI() = default;

    void AddCommand(const std::string& command, std::function<void()> func);
    void AddCommand(const std::string& command, std::function<void(const std::string&)> func);
    ///void AddCommand(std::string command, ..... );
    bool ExecuteIfCommand(const std::string& incomeMessage);
    std::map<std::string, std::unique_ptr<IExecutable>> commands;
};


#endif //CLI_H
