#ifndef CLI_H
#define CLI_H

#include <string>
#include <functional>
#include <map>
#include <regex>


/*---------------------------------CLI--------------------------------------------------------
 *I use the Command pattern and the CRTP pattern to implement a CLI for educational purposes.
 */

class IExecutable
{
public:
    virtual ~IExecutable() = default;
    virtual void Execute(const std::string& args) = 0;
};


template <class Derived>
class CommandBase : public IExecutable
{
public:
    void Execute(const std::string& args) override;
};

template <class Derived>
void CommandBase<Derived>::Execute(const std::string& args)
{
    static_cast<Derived*>(this)->ExecuteImpl(args);
}


class CommandNoArgs : public CommandBase<CommandNoArgs>
{
public:
    explicit CommandNoArgs(std::function<void()> func);
    ~CommandNoArgs();
    void ExecuteImpl(const std::string& args);


private:
    //void(*func)() = nullptr;
    std::function<void()> func = nullptr;
};

class CommandStrArgs : public CommandBase<CommandStrArgs>
{
public:
    CommandStrArgs(std::function<void(const std::string&)> func);
    ~CommandStrArgs();

    void ExecuteImpl(std::string args);


private:
    //void(*func)(const std::string&) = nullptr;
    std::function<void(const std::string&)> func = nullptr;
};



class CLI
{
public:
    CLI();
    ~CLI() = default;

    void AddCommand(const std::string& command, std::function<void()> func);
    void AddCommand(const std::string& command, std::function<void(const std::string&)> func);
    ///void AddCommand(std::string command, ..... );
    ///
    bool ExecuteIfCommand(const std::string& command);
    std::map<std::string, std::unique_ptr<IExecutable>> commands;
};


#endif //CLI_H
