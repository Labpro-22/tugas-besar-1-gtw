#include "core/TurnCommandProcessor.hpp"

#include <cctype>
#include <memory>

#include "core/commands/ActionCommandGroup.hpp"
#include "core/commands/DiceCommandGroup.hpp"
#include "core/commands/DisplayCommandGroup.hpp"
#include "core/commands/ITurnCommand.hpp"
#include "core/commands/TurnCommandContext.hpp"
#include "core/GameEngine.hpp"
#include "models/Dadu.hpp"
#include "models/Kartu/DeckKartu.hpp"
#include "models/Kartu/KartuKemampuanSpesial.hpp"
#include "models/Papan.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "utils/NimonspoliException.hpp"
#include "views/InputHandler.hpp"

TurnCommandProcessor::TurnCommandProcessor(
    GameEngine& engine,
    ConfigData* configData,
    Papan* papanPermainan,
    std::vector<Pemain*>& listPemain,
    LogTransaksiGame* logGame,
    DeckKartu<KartuKemampuanSpesial>* deckSkill,
    Dadu* dadu,
    ManagerPenjara* managerPenjara,
    PlayerActionService* actionService,
    int maxTurn
) : engine(engine),
    configData(configData),
    papanPermainan(papanPermainan),
    listPemain(listPemain),
    logGame(logGame),
    deckSkill(deckSkill),
    dadu(dadu),
    managerPenjara(managerPenjara),
    actionService(actionService),
    maxTurn(maxTurn) {
    commandGroups.push_back(std::make_unique<DisplayCommandGroup>());
    commandGroups.push_back(std::make_unique<ActionCommandGroup>());
    commandGroups.push_back(std::make_unique<DiceCommandGroup>());
}

std::string TurnCommandProcessor::normalizeCommand(const std::string& raw) {
    size_t a = raw.find_first_not_of(" \t\r\n");
    size_t b = raw.find_last_not_of(" \t\r\n");
    if (a == std::string::npos) return "";

    std::string line = raw.substr(a, b - a + 1);
    for (char& c : line) {
        if (c == '-') c = '_';
        c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }
    return line;
}

std::vector<std::string> TurnCommandProcessor::tokenize(const std::string& line) {
    std::vector<std::string> tok;
    std::string cur;
    for (char c : line) {
        if (c == ' ' || c == '\t') {
            if (!cur.empty()) {
                tok.push_back(cur);
                cur.clear();
            }
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) tok.push_back(cur);
    return tok;
}

bool TurnCommandProcessor::processNextCommand(Pemain* p, int currentTurn, TurnCommandState& state) {
    std::string line = InputHandler::promptLine("Perintah> ");
    if (line.empty()) return false;

    line = normalizeCommand(line);
    if (line.empty()) return false;

    std::vector<std::string> tok = tokenize(line);
    if (tok.empty()) return false;

    const std::string& cmd = tok[0];
    TurnCommandContext ctx{
        engine,
        p,
        currentTurn,
        maxTurn,
        configData,
        papanPermainan,
        listPemain,
        logGame,
        deckSkill,
        dadu,
        managerPenjara,
        actionService,
        state
    };

    for (const auto& group : commandGroups) {
        if (group->canHandle(cmd)) {
            return group->execute(cmd, tok, ctx);
        }
    }

    throw PerintahTidakDitemukanException();
}

