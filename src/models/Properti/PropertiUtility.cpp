#include "models/Properti/PropertiUtility.hpp"
#include "models/Properti/ManagerProperti.hpp"

PropertiUtility::PropertiUtility(const PropertiConfig& config):
    Properti(config){}

int PropertiUtility::hitungSewa(int dadu, ManagerProperti& manager) {
    if(getStatus() == StatusProperti::MORTGAGED) return 0;

    Pemain* pemilik = getPemilik();
    if(pemilik == nullptr) return 0;

    int jumlah = manager.hitungUtility(pemilik);
    int pengali = manager.getPengaliUtility(jumlah);

    return dadu * pengali;
}