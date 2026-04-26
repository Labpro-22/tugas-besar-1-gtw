#include "views/OutputHandler.hpp"
#include "models/Managers/ManagerPenjara.hpp"

// ======================== ManagerPenjara ========================
void ManagerPenjara::tryEscapeByDouble(Pemain &p, Dadu& d, PlayerActionService actionService) {
    if (p.getPercobaanKeluarPenjara() >= 3) {
        paksaBayar(p, actionService);
    }
    else {
        d.rollRandom();
        if (d.isDouble()) {
            p.setStatus(StatusPemain::ACTIVE);
        }
    }
}

void ManagerPenjara::escapeByFine(Pemain &p, PlayerActionService actionService) {
    actionService.transferMoney(&p, nullptr, actionService.getConfig()->getDendaPenjara());
    OutputHandler::cetakAksi(p.getUsername(), "membayar " + std::to_string(actionService.getConfig()->getDendaPenjara()) + " untuk keluar dari penjara!");
    p.setStatus(StatusPemain::ACTIVE);
}

void ManagerPenjara::paksaBayar(Pemain &p, PlayerActionService actionService) {
    OutputHandler::cetakAksi(p.getUsername(), "dipaksa bayar denda karena sudah berada di penjara selama 3 turn!");
    escapeByFine(p, actionService);
}
