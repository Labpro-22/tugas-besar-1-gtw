#include "models/Petak/PetakKartu.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "models/Kartu/KartuKesempatan.hpp"
#include "utils/LogTransaksiGame.hpp"
#include "utils/NimonspoliException.hpp"
#include <stdexcept>
#include <iostream>

PetakKartu::PetakKartu(int indeks, const std::string& kode, const std::string& nama, JenisKartu jenis, DeckKartu<Kartu>* deck) : PetakAksi(indeks, kode, nama), jenis(jenis), deck(deck) {}

void PetakKartu::onLanded(Pemain& pemain, PlayerActionService& svc) {
    if (jenis == JenisKartu::KESEMPATAN) {
        std::cout << "\nKamu mendarat di Petak Kesempatan!\n";
    } else {
        std::cout << "\nKamu mendarat di Petak Dana Umum!\n";
    }
    std::cout << "Mengambil kartu...\n";

    Kartu* kartu = nullptr;
    try {
        kartu = deck->ambilKartu();
    } catch (const std::runtime_error& e) {
        std::cout << " Deck kartu tidak tersedia: " << e.what() << "\n";
        std::cout << " Giliran kartu dilewati.\n";
        return;
    }

    std::cout << "Kartu: \"" << kartu->getNamaKartu() << "\"\n";

    KartuBebasPenjara* kbp = dynamic_cast<KartuBebasPenjara*>(kartu);
    if (kbp != nullptr) {
        try {
            std::cout << "Kartu Bebas Penjara disimpan di tanganmu untuk digunakan nanti.\n";
            svc.simpanKartuBebasPenjara(pemain, kartu);
        } catch (const SlotKartuPenuhException& e) {
            std::cout << "[!] Slot kartu penuh (" << e.gettipeError() << "). " << "Kartu Bebas Penjara dikembalikan ke deck.\n";
            deck->buangKartu(kartu);
        }
    } else {
        try {
            kartu->jalankanEfek(pemain, svc);
        } catch (const UangTidakCukupException& e) {
            std::cout << "Efek kartu gagal: uang tidak cukup (" << e.gettipeError() << ").\n";
        } catch (const NimonspoliException& e) {
            std::cout << "Efek kartu gagal: " << e.gettipeError() << "\n";
        }
        deck->buangKartu(kartu);
    }

    std::string jenisStr = (jenis == JenisKartu::KESEMPATAN) ? "KESEMPATAN" : "DANA_UMUM";
    svc.logAksi(pemain, "KARTU", jenisStr + " | " + kartu->getNamaKartu());
}