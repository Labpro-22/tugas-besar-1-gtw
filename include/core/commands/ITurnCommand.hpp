#pragma once

#include <string>
#include <vector>

struct TurnCommandContext;

class ITurnCommand {
public:
    virtual ~ITurnCommand() = default;
    virtual bool canHandle(const std::string& cmd) const = 0;
    virtual bool execute(const std::string& cmd, const std::vector<std::string>& tok, TurnCommandContext& ctx) = 0;
};

