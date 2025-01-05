//
// Created by pozni on 1/5/2025.
//

#include <iostream>
#include <utility>
#include "Command.h"


Command::Command(const char* key, Type type, ArgType argType)
    : _type{type},
      _argType{argType},
      _key{key}
{
}

Command::Command(std::string& key, Type type, ArgType argType)
    : _type{type},
      _argType{argType},
      _key{std::move(key)}
{
}

void Command::SetExecuteFunction(std::move_only_function<void()> func)
{
    _executeFunction = std::move(func);
}

void Command::Execute(const std::string& args) const
{
    if (_executeFunction)
    {
        try
        {
            _executeFunction();
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void Command::AddSubcommand(const Command& command)
{
    _subcommands.insert(command);
}

std::string Command::GetName() const
{
    return _key;
}


Command* Command::FindSubcommand(const std::string& name)
{
    auto it = std::find_if(_subcommands.begin(), _subcommands.end(),
    [name](const Command& command) { return command == name; });

    if (it != _subcommands.end())
    {
        return &(*const_cast<auto>(it));
    }
    else
    {
        return nullptr;
    }
}

Command::Type Command::GetType() const
{
    return _type;
}

bool Command::operator==(const std::string& name) const
{
    return _key == name;
}

bool Command::operator==(const char* name) const
{
    return _key == name;
}

bool Command::operator==(const Command& other) const
{
    return _key == other._key;
}
