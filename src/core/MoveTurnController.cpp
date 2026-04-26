#include "../../include/core/MoveTurnController.hpp"

void MovementController::moveSteps(Pemain *p, int step) {

}

void TurnController::randomizeTurn (vector<Pemain*> &listPemain) {

}

// int TurnController::getMaxTurn () {
//     return 
// }

int TurnController::getCurrentTurn () {
    
}

Pemain* TurnController::getPlayerTurn () {
    return urutanPemain[this->getCurrentTurn()];
}

void TurnController::startTurn () {
    for (Pemain* p : this->urutanPemain) {
        // suru jalan
    }
}

