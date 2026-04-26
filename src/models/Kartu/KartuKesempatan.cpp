#include "views/OutputHandler.hpp"
#include "models/Kartu/KartuKesempatan.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include <iostream>

void KartuPergiKeStasiunTerdekat::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    OutputHandler::cetakEfekKartu("Pergi ke stasiun terdekat", "Pemain bergerak ke stasiun terdekat.");
    svc.moveToNearestStation(pemain);
}

void KartuMundurTigaPetak::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    OutputHandler::cetakEfekKartu("Mundur 3 petak", "Pemain mundur 3 petak.");
    // negatif = mundur
    svc.movePlayerRelative(pemain, -3);
}

void KartuMasukPenjara::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    OutputHandler::cetakEfekKartu("Masuk Penjara", "Pemain masuk penjara.");
    svc.sendToJail(pemain);
}

void KartuBebasPenjara::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    if (pemain.getStatus() == StatusPemain::JAILED) {
        OutputHandler::cetakEfekKartu("Bebas dari Penjara", "Kamu dibebaskan dari penjara!");
        svc.escapeFromJailByCard(pemain);
    } else {
        OutputHandler::cetakEfekKartu("Bebas dari Penjara", "Kartu disimpan untuk digunakan nanti.");
        // kartu akan dikembalikan ke tangan
    }
    (void)svc;
}