#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <random>
#include "../models/ConfigData.hpp"
#include "../models/Pemain.hpp"
using namespace std;

class MovementController {
private :
    // papan Papan
    ConfigData configData;

public :
    void moveSteps (Pemain *p, int n);

    void teleport (Pemain *p, int idx);

    // Petak findNearestStation


};

class TurnController {
private :
    vector<Pemain*> urutanPemain; // player bisa dikasi id aja biar ini isinya id aja
    int currentTurn; // isinya index sekarang lagi turn siapa
    Pemain playerTurn;

public :
    void randomizeTurn (vector<Pemain*> &listPemain);

    int getMaxTurn ();

    int getCurrentTurn ();

    Pemain* getPlayerTurn ();

    void startTurn (); // per satu kali jalan
};