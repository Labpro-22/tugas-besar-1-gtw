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
#include "../models/Managers/ManagerPenjara.hpp"
#include "../models/Papan.hpp"
#include "../models/Dadu.hpp"
#include "../models/Managers/ManagerProperti.hpp"
#include "../models/Kartu/Kartu.hpp"
#include "../models/Kartu/DeckKartu.hpp"
#include "../models/Kartu/KartuKemampuanSpesial.hpp"
#include "../utils/LogTransaksiGame.hpp"
#include "../views/OutputHandler.hpp"
#include "BangkrutDanEndGame.hpp"
using namespace std;

// Forward declarations
class Petak;
class PetakProperti;
class PetakLahan;
class ManagerFestival;
template <typename T> class DeckKartu;

class GameEngine {
private :
    // Game Data
    ConfigData* configData;
    vector<Pemain*> listPemain;
    Papan* papanPermainan;
    LogTransaksiGame* logGame;
    DeckKartu<Kartu>* deckKartu;
    DeckKartu<KartuKemampuanSpesial>* deckSkill;
    Dadu* dadu;
    ManagerPenjara* managerPenjara;
    ManagerProperti* managerProperti;
    ManagerFestival* managerFestival;
    PlayerActionService* actionService;
    
    // Turn Management (dari TurnController)
    vector<Pemain*> urutanPemain;
    int currentTurnIndex;
    int currentGlobalTurn;
    int maxTurn;
    bool arahNormalTurn;

public :
    GameEngine();
    ~GameEngine();
    
    void startGame();
    
    // Methods buat TurnController
    void randomizeTurn();
    bool startTurn(Pemain* p, int currentTurn, int& doubleCount);

    // Getter dan Setter untuk Game Data
    ConfigData* getConfigData() const { return configData; }
    void setConfigData(ConfigData* config) { this->configData = config; }

    vector<Pemain*>& getListPemain() { return listPemain; }
    void setListPemain(vector<Pemain*>& pemain) { this->listPemain = pemain; }

    Papan* getPapanPermainan() const { return papanPermainan; }
    void setPapanPermainan(Papan* papan) { this->papanPermainan = papan; }

    LogTransaksiGame* getLogGame() const { return logGame; }
    void setLogGame(LogTransaksiGame* log) { this->logGame = log; }

    DeckKartu<Kartu>* getDeckKartu() const { return deckKartu; }
    void setDeckKartu(DeckKartu<Kartu>* deck) { this->deckKartu = deck; }

    DeckKartu<KartuKemampuanSpesial>* getDeckSkill() const { return deckSkill; }
    void setDeckSkill(DeckKartu<KartuKemampuanSpesial>* deck) { this->deckSkill = deck; }

    Dadu* getDadu() const { return dadu; }
    void setDadu(Dadu* d) { this->dadu = d; }

    ManagerPenjara* getManagerPenjara() const { return managerPenjara; }
    void setManagerPenjara(ManagerPenjara* manager) { this->managerPenjara = manager; }

    ManagerProperti* getManagerProperti() const { return managerProperti; }
    void setManagerProperti(ManagerProperti* manager) { this->managerProperti = manager; }

    ManagerFestival* getManagerFestival() const { return managerFestival; }
    void setManagerFestival(ManagerFestival* manager ) { this->managerFestival = manager; }

    PlayerActionService* getActionService() const { return actionService; }
    void setActionService(PlayerActionService* service) { this->actionService = service; }

    // Getter dan Setter untuk Turn Management
    vector<Pemain*>& getUrutanPemain() { return urutanPemain; }
    void setUrutanPemain(vector<Pemain*>& urutan) { this->urutanPemain = urutan; }

    int getCurrentTurnIndex() const { return currentTurnIndex; }
    void setCurrentTurnIndex(int index) { this->currentTurnIndex = index; }
    int& getCurrentTurnIndexRef() { return currentTurnIndex; }

    int getMaxTurn() const { return maxTurn; }
    void setMaxTurn(int max) { this->maxTurn = max; }

    int getCurrentGlobalTurn() const { return currentGlobalTurn; }
    void setCurrentGlobalTurn(int turn) { this->currentGlobalTurn = turn; }
    int& getCurrentGlobalTurnRef() { return currentGlobalTurn; }

    bool isArahNormalTurn() const { return arahNormalTurn; }
    void setArahNormalTurn(bool arah) { this->arahNormalTurn = arah; }

};