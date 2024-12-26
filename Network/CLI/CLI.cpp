//
// Created by pozni on 12/25/2024.
//

#include "CLI.h"
#include <iostream>


void CLI::execute(std::string command)
{
    if (commands.find(command) != commands.end())
    {
        commands[command]();
    }
    else
    {
        std::cout << "Command not found" << std::endl;
    }
}
