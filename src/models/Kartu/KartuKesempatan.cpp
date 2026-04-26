#include "models/Kartu/KartuKesempatan.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "utils/NimonspoliException.hpp"
#include <iostream>

void KartuPergiKeStasiunTerdekat::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "Kartu: \"Pergi ke stasiun terdekat.\"\n";
    try {
        svc.moveToNearestStation(pemain);
    } catch (const NimonspoliException& e) {
        std::cout << "Gagal memindahkan pemain ke stasiun terdekat: " << e.gettipeError() << "\n";
    }
}

void KartuMundurTigaPetak::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "Kartu: \"Mundur 3 petak.\"\n";
    try {
        svc.movePlayerRelative(pemain, -3);
    } catch (const NimonspoliException& e) {
        std::cout << "Gagal memundurkan pemain: " << e.gettipeError() << "\n";
    }
}

void KartuMasukPenjara::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "Kartu: \"Masuk Penjara.\"\n";
    try {
        svc.sendToJail(pemain);
    } catch (const NimonspoliException& e) {
        std::cout << "Gagal memasukkan pemain ke penjara: " << e.gettipeError() << "\n";
    }
}

void KartuBebasPenjara::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    // kartu ini tidak langsung dieksekusi saat diambil dari deck
    (void)pemain; (void)svc;
}