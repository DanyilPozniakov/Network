//
// Created by pozni on 1/5/2025.
//

#include "CommandManager.h"
#include <iostream>

void CommandManager::AddCommand(const Command& command)
{
    Command command("help", Command::Type::Command);
    {
        command.SetExecuteFunction([](){ std::cout << "Hello, I'm help - as main command!" << std::endl; });

        Command update("update", Command::Type::Command);
        command.AddSubcommand(update);
    }

}

void CommandManager::TryToExecute()
{
}

void CommandManager::Parse(int argc, char** argv)
{
}
