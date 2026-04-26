#include "models/Managers/ManagerFestival.hpp"
#include "models/Petak/PetakProperti.hpp"
#include "models/Pemain.hpp"
#include <iostream>

void ManagerFestival::applyFestival(PetakProperti* properti) {
    auto it = efekAktif.find(properti);
    if (it == efekAktif.end()) {
        efekAktif[properti] = {2, DURASI_DEFAULT};
        std::cout << "Efek festival aktif pada " << properti->getNama() << ": sewa x2 selama 3 giliran.\n";
    } else {
        auto& [pengali, durasi] = it->second;
        durasi = DURASI_DEFAULT;
        if (pengali < PENGALI_MAKS) {
            pengali *= 2;
        }
        std::cout << "Efek festival diperbarui pada " << properti->getNama() << ": sewa x" << pengali << " selama 3 giliran.\n";
    }
}

void ManagerFestival::updateDurasi(Pemain* pemilik) {
    for (auto it = efekAktif.begin(); it != efekAktif.end(); ) {
        PetakProperti* prop = it->first;
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

int ManagerFestival::getPengali(PetakProperti* properti) const {
    auto it = efekAktif.find(properti);
    return (it == efekAktif.end()) ? 1 : it->second.first;
}

int ManagerFestival::getSisaDurasi(PetakProperti* properti) const {
    auto it = efekAktif.find(properti);
    return (it == efekAktif.end()) ? 0 : it->second.second;
}

bool ManagerFestival::isAktif(PetakProperti* properti) const {
    auto it = efekAktif.find(properti);
    return it != efekAktif.end() && it->second.second > 0;
}

std::string ManagerFestival::getFestivalString(PetakProperti* properti) const {
    return std::to_string(getPengali(properti)) + " " + std::to_string(getSisaDurasi(properti));
}
