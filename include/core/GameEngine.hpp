#pragma once
#include "MoveTurnController.hpp"
#include "../models/Properti/ManagerProperti.hpp"
#include "../models/Petak/PetakFestival.hpp"
#include "../models/Pemain.hpp"
// include deck kartu
#include "../models/Kartu/DeckKartu.hpp"
#include "../models/Kartu/KartuKemampuanSpesial.hpp"
// include papan
// include log
#include "../utils/LogTransaksiGame.hpp"

class GameEngine {
private :
    vector<Pemain*> ListPemain;
    // Papan PapanPermainan;
    TurnController ManagerGiliran;
    // LogTransaksiGame LogGame
    LogTransaksiGame LogGame;
    // DeckKartu Deck
    DeckKartu<KartuKemampuanSpesial> deckKemampuan;
    ManagerProperti ManagerProperti;
    ManagerFestival ManagerFestival;

public :
    void startGame ();


    
    std::vector<Pemain*> getListPemain() { return ListPemain ;}
    int getJumlahPemain() { return static_cast<int>(ListPemain.size()); }
    int getTurnSaatIni() { return ManagerGiliran.getCurrentTurn(); }
    int getMaxTurn() { return ManagerGiliran.getMaxTurn(); }
    
    std::vector<Pemain*> getUrutanGiliran() { return ManagerGiliran.getUrutanPemain() ;}
    Pemain getGiliranAktif() { return ManagerGiliran.getCurrPemain() ;}

    const std::map<std::string, Properti*>& getDaftarProperti() { return ManagerProperti.getDaftarProperti(); }
    int getjumlahProperti() { return ManagerProperti.hitungJumlahProperti(); }

    std::string getJenisPropertiString(Properti* properti) { return properti->getJenisString(); }
    std::string getPemilikPropertiString(Properti* properti) { return properti->getPemilikString(); }
    std::string getStatusPropertiString(Properti* properti) { return properti->getStatusString(); }
    std::string getBangunanPropertiString(Properti* properti) { return properti->getBangunanString(); }
    std::string getFestivalPropertiString(Properti* properti) { return ManagerFestival.getFestivalString(properti); }

    int getJumlahKartuDeckKemampuan() { return deckKemampuan.ukuran(); }
    const std::vector<KartuKemampuanSpesial*>& getIsiDeckKemampuan() { return deckKemampuan.getTumpukan(); }

    int getJumlahEntriLog() { return static_cast<int>(LogGame.getLogs().size()); }
    const std::vector<LogTransaksiEntry>& getEntriLog() { return LogGame.getLogs(); }

};