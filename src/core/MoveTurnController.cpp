#include "../../include/core/MoveTurnController.hpp"

// void MovementController::moveSteps(Pemain *p, int step) {

// }

void TurnController::randomizeTurn (vector<Pemain*> &listPemain) {
    // vector<Pemain*> res;
    for (Pemain* p : listPemain) {
        this->urutanPemain.push_back(new Pemain(*p));  
    }

    random_shuffle(this->urutanPemain.begin(), this->urutanPemain.end());
}

// int TurnController::getMaxTurn () {
//     return 
// }

Pemain* TurnController::getCurrentTurn () {
    
}

// Pemain* TurnController::getPlayerTurn () {
//     return urutanPemain[this->getCurrentTurn()];
// }

void TurnController::startTurn (Pemain* p, PlayerActionService* actionService, Dadu* d, ManagerPenjara* mp) {
    // isi if else nya bisa diisi kalo udah ada command
    while (true) {
        if (1) { // atur dadu manual
            int a, b; // ini stub
            if (p->getStatus() == StatusPemain::ACTIVE) {
                d->rollManual(a, b);
                actionService->movePlayerRelative(*p, d->getTotalNilaiDadu());
                break;
            }
            else if (p->getStatus() == StatusPemain::JAILED) {
                std::cout << p->getUsername() << " sedang di penjara, tidak bisa melakukan atur dadu manual!\n";
            }
        }
        else if (1) { // roll dadu random
            if (p->getStatus() == StatusPemain::ACTIVE) {
                d->rollRandom();
                actionService->movePlayerRelative(*p, d->getTotalNilaiDadu());
            }
            else if (p->getStatus() == StatusPemain::JAILED) {
                mp->tryEscapeByDouble(*p, *d);
            }
            break;

        }
        else if (1) { // bayar fine (khusus kalo di penjara)
            if (p->getStatus() == StatusPemain::ACTIVE) {
                std::cout << p->getUsername() << " tidak sedang dipenjara, tidak bisa bayar denda!\n";
            }
            else if (p->getStatus() == StatusPemain::JAILED) {
                mp->paksaBayar(*p, *actionService);
                break;
            }
        }
        else if (1) { // cetak akta
            Formatter::cetakAktaProperti();
        }
        else if (1) { // cetak papan
            Formatter::cetakPapan();
        }
        else if (1) { // cetak properti
            Formatter::cetakPropertiPemain();
        }
        else if (1) { // gadai
            Formatter::tampilkanPropertiYangBisaDigadai();
        }
        else if (1) { // tebus
            Formatter::tampilkanPropertiYangBisaDitebus();
        }
        else if (1) { // bangun
            Formatter::tampilkanPropertiYangBisaDibangun();
        }
    }
    

}

