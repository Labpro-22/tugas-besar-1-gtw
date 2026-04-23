#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
#include "../models/ConfigData.hpp"
#include "../models/Pemain.hpp"
#include "../models/PlayerActionService.hpp"
#include "../models/ManagerPenjara.hpp"
#include "../views/Formatter.hpp"
using namespace std;

// class MovementController {
// private :
//     // papan Papan
//     ConfigData configData;

// public :
//     void moveSteps (Pemain *p, int n);

//     void teleport (Pemain *p, int idx);

//     // Petak findNearestStation


// };

class TurnController {
private :
    vector<Pemain*> urutanPemain; // player bisa dikasi id aja biar ini isinya id aja
    Pemain* currentTurn; // isinya index sekarang lagi turn siapa
    // Pemain playerTurn;

public :
    TurnController();

    void randomizeTurn (vector<Pemain*> &listPemain);

    // int getMaxTurn ();

    Pemain* getCurrentTurn ();

    // Pemain* getPlayerTurn ();

    void startTurn (Pemain* p, PlayerActionService* actionService, Dadu *d, ManagerPenjara *mp); // per satu kali jalan

    ~TurnController();
};