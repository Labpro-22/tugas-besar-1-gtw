#pragma once

#include <memory>
#include <string>
#include <vector>

class ITurnCommand;
class GameEngine;
class Pemain;
class ConfigData;
class Papan;
class LogTransaksiGame;
class Dadu;
class ManagerPenjara;
class PlayerActionService;
template <typename T> class DeckKartu;
class KartuKemampuanSpesial;

struct TurnCommandState {
    bool awalGiliran = true;
    bool sudahLemparDadu = false;
    bool adaAksiStateChange = false;
    bool extraTurn = false;
    int* doubleCount = nullptr;
};

class TurnCommandProcessor {
public:
    TurnCommandProcessor(
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
    );

    bool processNextCommand(Pemain* pemain, int currentTurn, TurnCommandState& state);

private:
    GameEngine& engine;
    ConfigData* configData;
    Papan* papanPermainan;
    std::vector<Pemain*>& listPemain;
    LogTransaksiGame* logGame;
    DeckKartu<KartuKemampuanSpesial>* deckSkill;
    Dadu* dadu;
    ManagerPenjara* managerPenjara;
    PlayerActionService* actionService;
    int maxTurn;
    std::vector<std::unique_ptr<ITurnCommand>> commandGroups;

    static std::string normalizeCommand(const std::string& line);
    static std::vector<std::string> tokenize(const std::string& line);
};

