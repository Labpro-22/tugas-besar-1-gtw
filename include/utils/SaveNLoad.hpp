#pragma once
#include <string>

// Forward declarations
class GameEngine;

class SaveNLoad{
public:
    void saveGameState(GameEngine& game, std::string filename);
    void loadGameState(GameEngine& game, std::string filename);
};