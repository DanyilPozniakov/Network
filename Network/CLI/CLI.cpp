//
// Created by pozni on 12/25/2024.
//

#include "CLI.h"
#include <iostream>
#include <utility>

// ...................................COMMANDS.....................................


CommandNoArgs::CommandNoArgs(std::function<void()> func)
:   CommandBase<CommandNoArgs>(),
    func(std::move(func))
{
}



void CommandNoArgs::ExecuteImpl(const std::string& args)
{
    if(func)
    {
        try
        {
            func();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}



CommandStrArgs::CommandStrArgs(std::function<void(const std::string&)> func)
:   CommandBase<CommandStrArgs>(),
    func(std::move(func))
{
}

void CommandStrArgs::ExecuteImpl(std::string args)
{
    if(func)
    {
        try
        {
            func(args);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}


//---------------------------------------------------------------------------------------
//-------------------------------------  CLI  -------------------------------------------

CLI::CLI()
{

}

void CLI::AddCommand(const std::string& command, std::function<void()> func)
{
    commands[command] = std::make_unique<CommandNoArgs>(std::move(func));
}

void CLI::AddCommand(const std::string& command, std::function<void(const std::string&)> func)
{
    commands[command] = std::make_unique<CommandStrArgs>(std::move(func));
}

bool CLI::ExecuteIfCommand(const std::string& command)
{
    if( auto comm = commands.find(command); comm != commands.end())
    {
        comm->second->Execute(command);
        return true;
    }
    return false;
}







