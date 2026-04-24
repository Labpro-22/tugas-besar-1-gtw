#include "../../include/core/GameEngine.hpp"

GameEngine::GameEngine() 
    : configData(nullptr), papanPermainan(nullptr), logGame(nullptr), deckKartu(nullptr), 
      dadu(nullptr), managerPenjara(nullptr), managerProperti(nullptr),
      actionService(nullptr), currentTurnIndex(0), maxTurn(0), arahNormalTurn(true) {
}

GameEngine::~GameEngine() {
    // Cleanup jika diperlukan
    if (configData) delete configData;
    for (Pemain* p : listPemain) {
        delete p;
    }
    for (Pemain* p : urutanPemain) {
        delete p;
    }
}

void GameEngine::startGame() {
    // Implementasi logic game utama

    // acak urutan playernya
    randomizeTurn();

    // iterasi sebanyak jumlah turn
    for (int currentTurn = 1; currentTurn <= maxTurn; currentTurn++) {

        // jalanin turn buat tiap player
        for (auto player : urutanPemain) {

            if (player->getStatus() != StatusPemain::BANKRUPT) { // yang bangkrut di skip
                continue;
            }

            if (SelesaiGame::sisaSatuOrang(listPemain)) { // kalo di tengah2 turn udah tinggal sisa satu orang, game langsung selesai
                SelesaiGame::tampilPanelSelesaiKarenaBangkrut();
                SelesaiGame::tampilkanPemenang (player);
                return;
            }


            // player bisa pilih mau liat2 akta, atau liat2 properti dia sendiri, ato mau jalan, ato mau make kartu
            startTurn (player, currentTurn);

            // habis turnya selesai normalnya player pindah tempat -> jalanin onlanded dari petak yang diinjek
            Petak* diTempati = papanPermainan->getPetak((const int)player->getPosisi());
            diTempati->onLanded(*player, *actionService);
            
        }
    }

    Pemain* pemenang = SelesaiGame::getPemenang (listPemain);
    SelesaiGame::tampilPanelSelesaiKarenaMaxTurn(maxTurn);
    SelesaiGame::tampilkanPemenang (pemenang);

}

void GameEngine::randomizeTurn() {
    for (Pemain* p : listPemain) {
        this->urutanPemain.push_back(new Pemain(*p));
    }
    random_shuffle(this->urutanPemain.begin(), this->urutanPemain.end());
}

void GameEngine::startTurn(Pemain* p, int currentTurn) {
    // isi if else nya bisa diisi kalo udah ada command
    while (true) {
        bool awalGiliran = true;
        if (1) { // atur dadu manual
            int a, b; // ini stub
            if (p->getStatus() == StatusPemain::ACTIVE) {
                dadu->rollManual(a, b);
                actionService->movePlayerRelative(*p, dadu->getTotalNilaiDadu());
                break;
            }
            else if (p->getStatus() == StatusPemain::JAILED) {
                std::cout << p->getUsername() << " sedang di penjara, tidak bisa melakukan atur dadu manual!\n";
            }
        }
        else if (1) { // roll dadu random
            if (p->getStatus() == StatusPemain::ACTIVE) {
                dadu->rollRandom();
                actionService->movePlayerRelative(*p, dadu->getTotalNilaiDadu());
            }
            else if (p->getStatus() == StatusPemain::JAILED) {
                managerPenjara->tryEscapeByDouble(*p, *dadu, *actionService);
            }
            break;

        }
        else if (1) { // bayar fine (khusus kalo di penjara)
            if (p->getStatus() == StatusPemain::ACTIVE) {
                std::cout << p->getUsername() << " tidak sedang dipenjara, tidak bisa bayar denda!\n";
            }
            else if (p->getStatus() == StatusPemain::JAILED) {
                managerPenjara->paksaBayar(*p, *actionService);
                break;
            }
        }
        else if (1) { // cetak akta
            // minta input kode properti dari user
            int kode = 0; // ini masih stub
            Properti *inginDicetak = managerProperti->getProperti(0);
            if (configData) {
                Formatter::cetakAktaProperti(inginDicetak, *configData);
            }
        }
        else if (1) { // cetak papan
            Formatter::cetakPapan(*papanPermainan, listPemain, currentTurn, maxTurn);
        }
        else if (1) { // cetak properti
            if (configData) {
                Formatter::cetakPropertiPemain(p, *configData);
            }
        }
        else if (1) { // gadai
            if (configData) {
                Formatter::tampilkanPropertiYangBisaDigadai(p, *configData);
            }
        }
        else if (1) { // tebus
            if (configData) {
                Formatter::tampilkanPropertiYangBisaDitebus(p, *configData);
            }
        }
        else if (1) { // bangun
            if (configData) {
                Formatter::tampilkanPropertiYangBisaDibangun(p, *managerProperti, *configData);
            }
        }
        else if (1) { // make kartu

        }
        else if (1) { // save game, cuma bisa di awal giliran
            if (awalGiliran) {
                // game kesave
            }
            else {
                std::cout << "Tidak bisa menyimpan game di tengah-tengah giliran!\n";
            }
        }
        awalGiliran = false;
    }
}
