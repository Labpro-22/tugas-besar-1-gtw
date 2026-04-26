#include "models/Pemain.hpp"
#include "models/Petak/PetakUtilitas.hpp"
#include "models/Managers/ManagerProperti.hpp"

PetakUtilitas::PetakUtilitas(int indeks, const std::string& kode, const std::string& nama, const PropertiConfig& config) :
    PetakProperti(indeks, kode, nama, config) {}

int PetakUtilitas::hitungSewa(int dadu, ManagerProperti& manager) {
    if(getStatus() == StatusProperti::MORTGAGED) return 0;

    Pemain* pemilik = getPemilik();
    if(pemilik == nullptr) return 0;

    int jumlah = manager.hitungUtility(pemilik);
    int pengali = manager.getPengaliUtility(jumlah);

    return dadu * pengali;
}

std::string PetakUtilitas::getJenisString() const {
    return "utility";
}
