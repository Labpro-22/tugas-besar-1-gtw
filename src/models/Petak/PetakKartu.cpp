#include "models/Petak/PetakKartu.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "models/Kartu/KartuKesempatan.hpp"
#include "utils/LogTransaksiGame.hpp"
#include <iostream>

PetakKartu::PetakKartu(int indeks, const std::string& kode, const std::string& nama, JenisKartu jenis, DeckKartu<Kartu>* deck) : PetakAksi(indeks, kode, nama), jenis(jenis), deck(deck) {}

void PetakKartu::onLanded(Pemain& pemain, PlayerActionService& svc) {
    if (jenis == JenisKartu::KESEMPATAN) {
        std::cout << "\nKamu mendarat di Petak Kesempatan!\n";
    } else {
        std::cout << "\nKamu mendarat di Petak Dana Umum!\n";
    }
    std::cout << "Mengambil kartu...\n";

    Kartu* kartu = deck->ambilKartu();
    std::cout << "Kartu: \"" << kartu->getNamaKartu() << "\"\n";
    KartuBebasPenjara* kbp = dynamic_cast<KartuBebasPenjara*>(kartu);
    if (kbp != nullptr) {
        std::cout << "Kartu Bebas Penjara disimpan di tanganmu untuk digunakan nanti.\n";
        svc.simpanKartuBebasPenjara(pemain, kartu);
    } else {
        kartu->jalankanEfek(pemain, svc);
        deck->buangKartu(kartu);
    }
    std::string jenisStr = (jenis == JenisKartu::KESEMPATAN) ? "KESEMPATAN" : "DANA_UMUM";
    svc.logAksi(pemain, "KARTU", jenisStr + " | " + kartu->getNamaKartu());
}