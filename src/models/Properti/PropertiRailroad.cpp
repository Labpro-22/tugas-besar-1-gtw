#include "models/Properti/PropertiRailroad.hpp"
#include "models/Managers/ManagerProperti.hpp"

PropertiRailroad::PropertiRailroad(const PropertiConfig& config):
    Properti(config){}

int PropertiRailroad::hitungSewa(int dadu, ManagerProperti& manager) {
    if(getStatus() == StatusProperti::MORTGAGED) return 0;

    Pemain* pemilik = getPemilik();
    if(pemilik == nullptr) return 0;

    int jumlah = manager.hitungRailroad(pemilik);

    return manager.getHargaSewaRailroad(jumlah);
}

std::string PropertiRailroad::getJenisString() const{
    return "railroad";
}
