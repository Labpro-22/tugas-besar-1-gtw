#include "models/Petak/PetakSpesial.hpp"

// ======================== PetakSpesial ========================
PetakSpesial::PetakSpesial(int indeks, const std::string& kode, const std::string& nama):
    PetakAksi(indeks, kode, nama){}

// ======================== PetakMulai ========================
PetakMulai::PetakMulai(int indeks, const std::string& kode, const std::string& nama):
    PetakSpesial(indeks, kode, nama){}

void PetakMulai::onLanded(Pemain &p, PlayerActionService actionService) {
    actionService.transferMoney(nullptr, &p, actionService.getConfig()->getGajiGo());
}

// ======================== PetakPenjara ========================
PetakPenjara::PetakPenjara(int indeks, const std::string& kode, const std::string& nama):
    PetakSpesial(indeks, kode, nama), manager_penjara(nullptr){}

void PetakPenjara::onLanded(Pemain &p, PlayerActionService actionService) {
    // ga ngapa2in lanjut aja biasa
}

// ======================== PetakPergiPenjara ========================
PetakPergiPenjara::PetakPergiPenjara(int indeks, const std::string& kode, const std::string& nama):
    PetakSpesial(indeks, kode, nama){}

void PetakPergiPenjara::onLanded(Pemain &p, PlayerActionService actionService) {
    masukkanKePenjara(p, actionService);
}

void PetakPergiPenjara::masukkanKePenjara(Pemain &p, PlayerActionService actionService) {
    actionService.sendToJail(p);
}

// ======================== PetakBebasParkir ========================
PetakBebasParkir::PetakBebasParkir(int indeks, const std::string& kode, const std::string& nama):
    PetakSpesial(indeks, kode, nama){}

void PetakBebasParkir::onLanded(Pemain &p, PlayerActionService actionService) {
    // ga ngapa2in juga
}