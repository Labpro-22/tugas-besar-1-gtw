#include "../../include/models/ManagerPenjara.hpp"

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
    std::cout << p.getUsername() << " membayar " << actionService.getConfig()->getDendaPenjara() << " untuk keluar dari penjara!\n";
    p.setStatus(StatusPemain::ACTIVE);
}

void ManagerPenjara::paksaBayar(Pemain &p, PlayerActionService actionService) {
    std::cout << p.getUsername() << " dipaksa bayar karena sudah berada di penjara selama 3 turn!\n";
    escapeByFine(p, actionService);
}
