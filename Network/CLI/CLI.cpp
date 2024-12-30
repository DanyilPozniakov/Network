//
// Created by pozni on 12/25/2024.
//

#include "CLI.h"
#include <iostream>
#include <utility>


Command::Command(void (*func)()) : _func(func)
{
    _type = FuncType::NoArgs;
}

Command::Command(void (*func)(std::string)) : _func(func)
{
    _type = FuncType::StringArg;
}

Command::Command(void (*func)(std::smatch)): _func(func)
{
    _type = FuncType::SMatchArg;
}

void Command::Execute(const std::string& args) const
{
    switch (_type)
    {
    case FuncType::NoArgs:
        {
            void(*func)() = reinterpret_cast<void (*)()>(_func);
            func();
        }
    case FuncType::StringArg:
        {
            void(*func)(std::string) = reinterpret_cast<void (*)(std::string)>(_func);
            func(args);
        }

    case FuncType::SMatchArg:
        {

        }
    default: ;
    }
}


/* #######  CLI ######## */
bool CLI::ExecuteIfCommand(const std::string& command)
{
    if (auto comm = commands.find(command); comm != commands.end())
    {
        comm->second.Execute(command);
        return true;
    }
    return false;
}


/* #######  CLI ######## */

