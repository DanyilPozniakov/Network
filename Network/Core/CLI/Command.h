//
// Created by pozni on 1/5/2025.
//

#ifndef COMMAND_H
#define COMMAND_H

#include <functional>
#include <string>
#include <optional>
#include <unordered_set>

class Command
{
public:
    enum class Type
    {
        None,
        Command,
        Brief,
        Full
    };

    enum class ArgType
    {
        None,
        String,
    };

    struct Hash
    {
        size_t operator()(const Command& command) const
        {
            return std::hash<std::string>()(command.GetName());
        }
    };

    explicit Command(const char* key, Type type, ArgType argType = ArgType::None);
    explicit Command(std::string& key, Type type, ArgType argType = ArgType::None);

    void SetExecuteFunction(std::move_only_function<void()> func);
    void Execute(const std::string& args) const;

    void AddSubcommand(const Command& command);
    [[nodiscard]] std::string GetName() const;
    [[nodiscard]] Type GetType() const;
    [[nodiscard]] Command* FindSubcommand(const std::string& name);

    [[nodiscard]] bool operator==(const std::string& name) const;
    [[nodiscard]] bool operator==(const char* name) const;
    [[nodiscard]] bool operator==(const Command& other) const;

private:
    std::function<void()> _executeFunction = nullptr;
    std::unordered_set<Command,Hash> _subcommands;

    Type _type = Type::None;
    ArgType _argType = ArgType::None;

    std::string _key;
    std::optional<std::string> _value = std::nullopt;
};


#endif //COMMAND_H
