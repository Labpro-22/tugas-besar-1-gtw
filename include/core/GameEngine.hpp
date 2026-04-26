#pragma once
#include "MoveTurnController.hpp"
#include "../models/Pemain.hpp"
// include deck kartu
// include papan
// include log

class GameEngine {
private :
    vector<Pemain*> ListPemain;
    // Papan PapanPermainan;
    TurnController ManagerGiliran;
    // LogTransaksiGame LogGame
    // DeckKartu Deck

public :
    void startGame ();



};