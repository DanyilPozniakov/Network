#ifndef CLI_H
#define CLI_H

#include <string>
#include <functional>
#include <map>
#include <regex>

enum class FuncType
{
    NoArgs,
    StringArg,
    SMatchArg
};

class IExecutable
{
    virtual ~IExecutable() = default;
    virtual void Execute(const std::string& args) const = 0;
};

class Command : public IExecutable
{
public:
    explicit Command(void (*func)());
    explicit Command(void (*func)(std::string));
    explicit Command(void (*func)(std::smatch));

    void Execute(const std::string& args) const override;

private:
    void* _func = nullptr;
    FuncType _type = FuncType::NoArgs;
};



class CLI
{
public:
    CLI() = default;
    ~CLI() = default;

    bool ExecuteIfCommand(const std::string& command);
    std::map<std::string, Command> commands;

};


#endif //CLI_H
