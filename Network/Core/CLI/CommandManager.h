//
// Created by pozni on 1/5/2025.
//

#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H
#include "Command.h"


class CommandManager {
public:
    void AddCommand(const Command& command);
    void TryToExecute();

protected:
    void Parse(int argc, char** argv);

private:
    std::unordered_set<Command> _commands;
    Command* _target = nullptr;
};



#endif //COMMANDMANAGER_H
