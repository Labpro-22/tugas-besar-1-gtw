#pragma once

#include "core/commands/ITurnCommand.hpp"

class ActionCommandGroup : public ITurnCommand {
public:
    bool canHandle(const std::string& cmd) const override;
    bool execute(const std::string& cmd, const std::vector<std::string>& tok, TurnCommandContext& ctx) override;
};

