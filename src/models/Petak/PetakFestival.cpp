#include "models/Petak/PetakFestival.hpp"
#include "models/Properti/Properti.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "utils/NimonspoliException.hpp"
#include <stdexcept>
#include <iostream>
#include <limits>

static int mintaPilihanProperti(int maks) {
    int pilihan = -1;
    while (true) {
        std::cout << "Pilih properti (1-" << maks << "): ";
        if (std::cin >> pilihan && pilihan >= 1 && pilihan <= maks) break;
        std::cout << "Input tidak valid. Coba lagi.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return pilihan;
}

PetakFestival::PetakFestival(int indeks, const std::string& kode, const std::string& nama, ManagerFestival* mgr) : PetakAksi(indeks, kode, nama), managerFestival(mgr) {
    if (mgr == nullptr) throw std::invalid_argument("PetakFestival: ManagerFestival tidak boleh null."); 
}

void PetakFestival::onLanded(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "\nKamu mendarat di petak Festival!\n";
    const auto& aset = pemain.getAsetPemain();
    if (aset.empty()) {
        std::cout << "Kamu tidak memiliki properti.\n";
        return;
    }

    std::cout << "Daftar properti milikmu:\n";
    for (size_t i = 0; i < aset.size(); ++i) {
        Properti* p = aset[i];
        std::cout << (i + 1) << ". " << p->getNamaProperti() << " (" << p->getKode() << ")";
        try {
            if (managerFestival->isAktif(p)) {
                std::cout << " [festival x" << managerFestival->getPengali(p) << ", sisa " << managerFestival->getSisaDurasi(p) << " giliran]";
            }
        } catch (const std::invalid_argument& e) {
            std::cout << " [error baca status festival: " << e.what() << "]";
        }
        std::cout << "\n";
    }

    int pilihan = mintaPilihanProperti(static_cast<int>(aset.size()));
    Properti* target = aset[pilihan - 1];
    try {
        managerFestival->applyFestival(target);
    } catch (const std::invalid_argument& e) {
        std::cout << "Gagal mengaktifkan festival: " << e.what() << "\n";
        return;
    }

    try {
        std::string detail = target->getKode() + " festival x" + std::to_string(managerFestival->getPengali(target)) + " durasi " + std::to_string(managerFestival->getSisaDurasi(target));
        svc.logAksi(pemain, "FESTIVAL", detail);
    } catch (const std::invalid_argument& e) {
        std::cout << "Gagal membaca info festival untuk log: " << e.what() << "\n";
    }
}