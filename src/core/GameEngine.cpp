#include "../../include/core/GameEngine.hpp"
#include "../../include/utils/NimonspoliException.hpp"
#include "../../include/views/InputHandler.hpp"

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
    // urutanPemain berisi pointer yang sama dengan listPemain (bukan owner)
    urutanPemain.clear();
}

void GameEngine::startGame() {
    // Implementasi logic game utama

    // acak urutan playernya
    randomizeTurn();

    // iterasi sebanyak jumlah turn
    for (currentGlobalTurn = 1; currentGlobalTurn <= maxTurn; currentGlobalTurn++) {

        // jalanin turn buat tiap player
        for (auto player : urutanPemain) {

            if (player->getStatus() == StatusPemain::BANKRUPT) { // yang bangkrut di skip
                continue;
            }

            if (SelesaiGame::sisaSatuOrang(listPemain)) { // kalo di tengah2 turn udah tinggal sisa satu orang, game langsung selesai
                SelesaiGame::tampilPanelSelesaiKarenaBangkrut();
                SelesaiGame::tampilkanPemenang (player);
                return;
            }


            // player bisa pilih mau liat2 akta, atau liat2 properti dia sendiri, ato mau jalan, ato mau make kartu
            startTurn (player, currentGlobalTurn);

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
    urutanPemain = listPemain; // pointer asli, state tetap konsisten
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(urutanPemain.begin(), urutanPemain.end(), gen);
}

void GameEngine::startTurn(Pemain* p, int currentTurn) {
    // isi if else nya bisa diisi kalo udah ada command
    while (true) {
        try {
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
            PetakProperti *inginDicetak = managerProperti->getProperti(0);
            if (configData) {
                OutputHandler::cetakAktaProperti(inginDicetak, *configData);
            }
        }
        else if (1) { // cetak papan
            OutputHandler::cetakPapan(*papanPermainan, listPemain, currentTurn, maxTurn);
        }
        else if (1) { // cetak properti
            if (configData) {
                OutputHandler::cetakPropertiPemain(p, *configData);
            }
        }
        else if (1) { // gadai
            if (configData) {
                OutputHandler::tampilkanPropertiYangBisaDigadai(p, *configData);
            }
        }
        else if (1) { // tebus
            if (configData) {
                OutputHandler::tampilkanPropertiYangBisaDitebus(p, *configData);
            }
        }
        else if (1) { // bangun
            if (configData) {
                OutputHandler::tampilkanPropertiYangBisaDibangun(p, *managerProperti, *configData);
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
        } catch (const NimonspoliException& e) {
            OutputHandler::cetakError(e.what());
            OutputHandler::cetakPesan("Silakan coba aksi lain.");
        }
    }
}
