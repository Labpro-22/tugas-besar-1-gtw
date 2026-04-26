#include "models/Petak/ManagerFestival.hpp"
#include "models/Properti/Properti.hpp"
#include "models/Pemain.hpp"
#include <iostream>

void ManagerFestival::applyFestival(Properti* properti) {
    auto it = efekAktif.find(properti);
    if (it == efekAktif.end()) {
        efekAktif[properti] = {2, DURASI_DEFAULT};
        std::cout << "Efek festival aktif pada " << properti->getNamaProperti() << ": sewa x2 selama 3 giliran.\n";
    } else {
        auto& [pengali, durasi] = it->second;
        durasi = DURASI_DEFAULT;
        if (pengali < PENGALI_MAKS) {
            pengali *= 2;
        }
        std::cout << "Efek festival diperbarui pada " << properti->getNamaProperti() << ": sewa x" << pengali << " selama 3 giliran.\n";
    }
}

void ManagerFestival::updateDurasi(Pemain* pemilik) {
    for (auto it = efekAktif.begin(); it != efekAktif.end(); ) {
        Properti* prop = it->first;
        if (prop->getPemilik() == pemilik) {
            it->second.second--;
            if (it->second.second <= 0) {
                it = efekAktif.erase(it);
            } else {
                ++it;
            }
        } else {
            ++it;
        }
    }
}

int ManagerFestival::getPengali(Properti* properti) const {
    auto it = efekAktif.find(properti);
    return (it == efekAktif.end()) ? 1 : it->second.first;
}

int ManagerFestival::getSisaDurasi(Properti* properti) const {
    auto it = efekAktif.find(properti);
    return (it == efekAktif.end()) ? 0 : it->second.second;
}

bool ManagerFestival::isAktif(Properti* properti) const {
    auto it = efekAktif.find(properti);
    return it != efekAktif.end() && it->second.second > 0;
}