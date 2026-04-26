#include "models/Pemain.hpp"
#include "models/Petak/PetakStasiun.hpp"
#include "models/Managers/ManagerProperti.hpp"

PetakStasiun::PetakStasiun(int indeks, const std::string& kode, const std::string& nama, const PropertiConfig& config) :
    PetakProperti(indeks, kode, nama, config) {}

int PetakStasiun::hitungSewa(int dadu, ManagerProperti& manager) {
    if(getStatus() == StatusProperti::MORTGAGED) return 0;

    Pemain* pemilik = getPemilik();
    if(pemilik == nullptr) return 0;

    int jumlah = manager.hitungRailroad(pemilik);

    return manager.getHargaSewaRailroad(jumlah);
}

std::string PetakStasiun::getJenisString() const {
    return "railroad";
}
