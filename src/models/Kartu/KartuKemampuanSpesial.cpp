#include "views/OutputHandler.hpp"
#include "models/Kartu/KartuKemampuanSpesial.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include <iostream>
#include <string>
#include <limits>

MoveCard::MoveCard(int langkah) : KartuKemampuanSpesial("MoveCard"), langkah(langkah) {}
void MoveCard::gunakan(Pemain& pemain, PlayerActionService& svc) {
    OutputHandler::cetakEfekKartu("MoveCard", "Maju " + std::to_string(langkah) + " petak.");
    svc.movePlayerRelative(pemain, langkah);
}
std::string MoveCard::getDeskripsi() const {
    return "MoveCard - Maju " + std::to_string(langkah) + " Petak";
}
std::string MoveCard::getNilaiState() const {
    return std::to_string(langkah);
}

DiscountCard::DiscountCard(int persen) : KartuKemampuanSpesial("DiscountCard"), persenDiskon(persen), sisaDurasi(0) {}
void DiscountCard::gunakan(Pemain& pemain, PlayerActionService& svc) {
    OutputHandler::cetakEfekKartu("DiscountCard", "Diskon " + std::to_string(persenDiskon) + "% aktif. Berlaku selama 1 giliran ini.");
    pemain.setDiscountAktif(true, persenDiskon);
    sisaDurasi = 1;
    (void)svc;
}
std::string DiscountCard::getDeskripsi() const {
    return "DiscountCard - Diskon " + std::to_string(persenDiskon) + "% selama 1 giliran";
}
std::string DiscountCard::getNilaiState() const {
    return std::to_string(persenDiskon);
}
std::string DiscountCard::getSisaDurasiState() const {
    return std::to_string(sisaDurasi);
}

ShieldCard::ShieldCard() : KartuKemampuanSpesial("ShieldCard") {}
void ShieldCard::gunakan(Pemain& pemain, PlayerActionService& svc) {
    OutputHandler::cetakEfekKartu("ShieldCard", "Perisai aktif! Kebal dari semua tagihan dan sanksi selama giliran ini.");
    pemain.setShieldAktif(true);
    (void)svc;
}
std::string ShieldCard::getDeskripsi() const {
    return "ShieldCard - Kebal tagihan/sanksi selama 1 giliran";
}

TeleportCard::TeleportCard() : KartuKemampuanSpesial("TeleportCard") {}
void TeleportCard::gunakan(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "[TeleportCard] Masukkan kode petak tujuan: ";
    std::string input;
    std::cin >> input;
    // validasi dan teleport dilakukan di PlayerActionService
    svc.teleportByInput(pemain, input);
}
std::string TeleportCard::getDeskripsi() const {
    return "TeleportCard - Pindah ke petak manapun";
}

LassoCard::LassoCard() : KartuKemampuanSpesial("LassoCard") {}
void LassoCard::gunakan(Pemain& pemain, PlayerActionService& svc) {
    OutputHandler::cetakEfekKartu("LassoCard", "Menarik pemain yang berada paling dekat di depan kamu ke posisi kamu.");
    // svc mencari pemain aktif yang terdekat
    svc.pullPlayerAhead(pemain);
}
std::string LassoCard::getDeskripsi() const {
    return "LassoCard - Menarik lawan di depan ke posisi kamu";
}

DemolitionCard::DemolitionCard() : KartuKemampuanSpesial("DemolitionCard") {}
void DemolitionCard::gunakan(Pemain& pemain, PlayerActionService& svc) {
    OutputHandler::cetakEfekKartu("DemolitionCard", "Hancurkan semua bangunan milik pemain lawan.");
    // svc menampilkan daftar pemain dan properti bergambar, lalu dieksekusi
    svc.demolishOpponentProperty(pemain);
}
std::string DemolitionCard::getDeskripsi() const {
    return "DemolitionCard - Hancurkan semua bangunan properti lawan";
}


RotasiKartuCard::RotasiKartuCard() : KartuKemampuanSpesial("RotasiKartuCard") {}
void RotasiKartuCard::gunakan(Pemain& pemain, PlayerActionService& svc) {
    OutputHandler::cetakEfekKartu("RotasiKartuCard", "Semua kartu di tangan setiap pemain berputar ke pemain berikutnya!");
    svc.rotateAllHandCards(pemain);
}
std::string RotasiKartuCard::getDeskripsi() const {
    return "RotasiKartuCard - Kartu tangan semua pemain berputar searah giliran";
}

ReverseCard::ReverseCard() : KartuKemampuanSpesial("ReverseCard") {}
void ReverseCard::gunakan(Pemain& pemain, PlayerActionService& svc) {
    OutputHandler::cetakEfekKartu("ReverseCard", "Arah urutan giliran dibalik!");
    // di svc, TurnController::reverseOrder()
    svc.reverseTurnOrder(pemain);
}
std::string ReverseCard::getDeskripsi() const {
    return "ReverseCard - Membalik arah urutan giliran bermain";
}

PenjaraKanCard::PenjaraKanCard() : KartuKemampuanSpesial("PenjaraKanCard") {}
void PenjaraKanCard::gunakan(Pemain& pemain, PlayerActionService& svc) {
    OutputHandler::cetakEfekKartu("PenjaraKanCard", "Pilih pemain lawan yang ingin dipenjara.");
    svc.jailOpponent(pemain);
}
std::string PenjaraKanCard::getDeskripsi() const {
    return "PenjaraKanCard - Kirim pemain lawan pilihanmu ke penjara";
}
