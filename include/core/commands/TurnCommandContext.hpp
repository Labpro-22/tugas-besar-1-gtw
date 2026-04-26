#pragma once

#include <string>
#include <vector>

#include "core/TurnCommandProcessor.hpp"

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

struct TurnCommandContext {
    GameEngine& engine;
    Pemain* pemain;
    int currentTurn;
    int maxTurn;
    ConfigData* configData;
    Papan* papanPermainan;
    std::vector<Pemain*>& listPemain;
    LogTransaksiGame* logGame;
    DeckKartu<KartuKemampuanSpesial>* deckSkill;
    Dadu* dadu;
    ManagerPenjara* managerPenjara;
    PlayerActionService* actionService;
    TurnCommandState& state;
};

