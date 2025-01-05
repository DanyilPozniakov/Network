//
// Created by pozni on 12/25/2024.
//

#include "CLI.h"
#include <iostream>
#include <utility>
#include <sstream>
#include <string>





//---------------------------------------------------------------------------------------
//-------------------------------------  CLI  -------------------------------------------

CLI::CLI()
{

}

void CLI::AddCommand(const std::string& command, std::move_only_function<void()> func)
{
    commands[command] = std::make_unique<CommandNoArgs>(std::move(func));
}

void CLI::AddCommand(const std::string& command, std::function<std::move_only_function<void()> func)
{
    commands[command] = std::make_unique<CommandStrArgs>(std::move(func));
}

bool CLI::ExecuteIfCommand(const std::string& incomeMessage)
{
    std::stringstream ss(incomeMessage);
    std::string commandName;
    ss >> commandName;

    std::string args = ss.str();

    //TODO: parseArgs(std::string& args)


    if( auto comm = commands.find(commandName); comm != commands.end())
    {
        comm->second->Execute(args);
        return true;
    }
    return false;
}







