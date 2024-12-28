//
// Created by pozni on 12/25/2024.
//

#include "CLI.h"
#include <iostream>
#include <utility>


bool CLI::ExecuteIfCommand(const std::string& command)
{
    if(auto comm = commands.find(command); comm != commands.end())
    {
        execute(comm->second);
        return true;
    }
    else
    {
        return false;
    }
}

void CLI::AddCommand(const std::string& command, std::function<void()> func)
{
    commands[command] = std::move(func);
}

void CLI::execute(const std::function<void()>& func)
{
    try
    {
        func();
    }
    catch (std::exception& e)
    {
        std::cerr << "Error in CLI->execute():" << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown error in CLI->execute()" << std::endl;
    }
}
