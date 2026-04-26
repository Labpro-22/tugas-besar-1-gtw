#include "models/Managers/ManagerProperti.hpp"

PropertiStreet::PropertiStreet(const PropertiConfig& config) : 
    Properti(config), level(0), tabelHargaSewa(config.getHargaSewa()){
        warna = convertWarna(config.getWarna());
}
bool PropertiStreet::punyaHotel() const{
    return level == 5;
}

int PropertiStreet::hitungSewa(int dadu, ManagerProperti& manager){
    if(status == StatusProperti::MORTGAGED) return 0;

    if(level == 0){
        int base = config->getHargaSewa(0);

        Pemain* pemilik = getPemilik();
        if(pemilik == nullptr) return 0;

        if(manager.isMonopoly(pemilik, warna)){
            return base * 2;
        }
        return base;
    }

    return config->getHargaSewa(level);
}

void PropertiStreet::bangun(ManagerProperti& manager){
    if(level >= 5) return;

    if(manager.isMonopoly(getPemilik(), warna) && manager.isBisaBangun(this)){
        level++;
    }
} 

int PropertiStreet::getJumlahBangunan() const{
    return level;
}

int PropertiStreet::getHargaBangun() const{
    if(level < 4){ // untuk bangun rumah
        return config->getHargaBangunRumah();
    }
    else if(level == 4){ // untuk upgrade ke hotel
        return config->getHargaBangunHotel();
    }
    return 0;
}

std::string PropertiStreet::getJenisString() const{
    return "street";
}

std::string PropertiStreet::getBangunanString() const{
    return getJumlahBangunanString();
}

std::string PropertiStreet::getJumlahBangunanString() const{
    if (punyaHotel()) {
        return "H";
    }

    return std::to_string(level);
}

PropertiStreet::ColorGroup PropertiStreet::getWarna() const{
    return warna;
}

PropertiStreet::ColorGroup PropertiStreet::convertWarna(const std::string& warnaStr){
    if(warnaStr == "COKLAT") return PropertiStreet::ColorGroup::COKLAT;
    if(warnaStr == "BIRU_MUDA") return PropertiStreet::ColorGroup::BIRU_MUDA;
    if(warnaStr == "MERAH_MUDA") return PropertiStreet::ColorGroup::MERAH_MUDA;
    if(warnaStr == "ORANGE") return PropertiStreet::ColorGroup::ORANGE;
    if(warnaStr == "MERAH") return PropertiStreet::ColorGroup::MERAH;
    if(warnaStr == "KUNING") return PropertiStreet::ColorGroup::KUNING;
    if(warnaStr == "HIJAU") return PropertiStreet::ColorGroup::HIJAU;
    if(warnaStr == "BIRU_TUA") return PropertiStreet::ColorGroup::BIRU_TUA;
    return PropertiStreet::ColorGroup::DEFAULT;
}

void PropertiStreet::turunkanBangunan(){
    if(level > 0) level--;
}
