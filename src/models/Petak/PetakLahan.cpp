#include "models/Pemain.hpp"
#include "models/Petak/PetakLahan.hpp"
#include "models/Managers/ManagerProperti.hpp"

PetakLahan::PetakLahan(int indeks, const std::string& kode, const std::string& nama, const PropertiConfig& config) : 
    PetakProperti(indeks, kode, nama, config), level(0), tabelHargaSewa(config.getHargaSewa()){
        warna = convertWarna(config.getWarna());
}

bool PetakLahan::punyaHotel() const{
    return level == 5;
}

int PetakLahan::hitungSewa(int dadu, ManagerProperti& manager){
    if(status == StatusProperti::MORTGAGED) return 0;

    if(level == 0){
        int base = config->getHargaSewa(0);

        Pemain* pemilik = getPemilik();
        if(pemilik == nullptr) return 0;

        if(manager.isMonopoly(pemilik, getWarnaString())){ // ManagerProperti uses string for simplicity
            return base * 2;
        }
        return base;
    }

    return config->getHargaSewa(level);
}

void PetakLahan::bangun(ManagerProperti& manager){
    if(level >= 5) return;

    if(manager.isMonopoly(getPemilik(), getWarnaString()) && manager.isBisaBangun(this)){
        level++;
    }
} 

int PetakLahan::getJumlahBangunan() const{
    return level;
}

int PetakLahan::getHargaBangun() const{
    if(level < 4){ // untuk bangun rumah
        return config->getHargaBangunRumah();
    }
    else if(level == 4){ // untuk upgrade ke hotel
        return config->getHargaBangunHotel();
    }
    return 0;
}

std::string PetakLahan::getJenisString() const{
    return "street";
}

std::string PetakLahan::getBangunanString() const{
    return getJumlahBangunanString();
}

std::string PetakLahan::getJumlahBangunanString() const{
    if (punyaHotel()) {
        return "H";
    }

    return std::to_string(level);
}

PetakLahan::ColorGroup PetakLahan::getWarna() const{
    return warna;
}

std::string PetakLahan::getWarnaString() const{
    return config->getWarna();
}

PetakLahan::ColorGroup PetakLahan::convertWarna(const std::string& warnaStr){
    if(warnaStr == "COKLAT") return PetakLahan::ColorGroup::COKLAT;
    if(warnaStr == "BIRU_MUDA") return PetakLahan::ColorGroup::BIRU_MUDA;
    if(warnaStr == "MERAH_MUDA") return PetakLahan::ColorGroup::MERAH_MUDA;
    if(warnaStr == "ORANGE") return PetakLahan::ColorGroup::ORANGE;
    if(warnaStr == "MERAH") return PetakLahan::ColorGroup::MERAH;
    if(warnaStr == "KUNING") return PetakLahan::ColorGroup::KUNING;
    if(warnaStr == "HIJAU") return PetakLahan::ColorGroup::HIJAU;
    if(warnaStr == "BIRU_TUA") return PetakLahan::ColorGroup::BIRU_TUA;
    return PetakLahan::ColorGroup::DEFAULT;
}

void PetakLahan::turunkanBangunan(){
    if(level > 0) level--;
}
