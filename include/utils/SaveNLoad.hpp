#pragma once
#include "../core/GameEngine.hpp"

class SaveNLoad{
private:

public:
    void saveGameState(GameEngine game, std::string filename);

    std::vector<std::string> saveStatePemain(GameEngine& game);
    std::vector<std::string> saveStateGiliran(GameEngine& game);
    std::vector<std::string> saveStateProperti(GameEngine& game);
    std::vector<std::string> saveStateDeck(GameEngine& game);
    std::vector<std::string> saveStateLog(GameEngine& game);

    GameEngine loadGameState(std::string filename);

    std::vector<std::string> loadStatePemain(GameEngine& game);
    std::vector<std::string> loadStateGiliran(GameEngine& game);
    std::vector<std::string> saveStateProperti(GameEngine& game);
    std::vector<std::string> saveStateDeck(GameEngine& game);
    std::vector<std::string> saveStateLog(GameEngine& game);
};