
#ifndef CLI_H
#define CLI_H

#include <string>
#include <functional>
#include <map>

class CLI {
public:
    CLI() = default;
    ~CLI() = default;

    bool ExecuteIfCommand(const std::string& command);
    void AddCommand(const std::string& command, std::function<void()> func);
    std::map<std::string, std::function<void()>> commands;

protected:
    static void execute(const std::function<void()>& func);


};



#endif //CLI_H
