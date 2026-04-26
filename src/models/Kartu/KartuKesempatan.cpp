#include "models/Kartu/KartuKesempatan.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include <iostream>

void KartuPergiKeStasiunTerdekat::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "Kartu: \"Pergi ke stasiun terdekat.\"\n";
    svc.moveToNearestStation(pemain);
}

void KartuMundurTigaPetak::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "Kartu: \"Mundur 3 petak.\"\n";
    // negatif = mundur
    svc.movePlayerRelative(pemain, -3);
}

void KartuMasukPenjara::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "Kartu: \"Masuk Penjara.\"\n";
    svc.sendToJail(pemain);
}

void KartuBebasPenjara::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    // kartu ini tidak langsung dieksekusi saat diambil dari deck
    (void)pemain; (void)svc;
}