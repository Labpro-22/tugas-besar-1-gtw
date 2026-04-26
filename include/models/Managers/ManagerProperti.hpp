#pragma once
#include "../Petak/PetakProperti.hpp"
#include "../Petak/PetakLahan.hpp"
#include "../Petak/PetakStasiun.hpp"
#include "../Petak/PetakUtilitas.hpp"
#include "../ConfigData.hpp"
#include <vector>
#include <string>

class ManagerProperti{
protected:
    std::map<int, PetakProperti*> daftarProperti;
    const ConfigData& configData;
    
public:
    ManagerProperti(const ConfigData& configData);

    void registerProperti(PetakProperti* p);

    bool isMonopoly(Pemain* pemain, std::string warna);

    bool isBisaBangun(PetakLahan* target); // level < 5 dan selisih jumlah rumah antar petak tidak lebih dari 1

    void jualSemuaBangunan(Pemain* pemain, std::string warna);

    int hitungLikuidasi(Pemain* pemain);

    bool isPunyaSemuaDalamGrup(Pemain* pemain, std::string warna);

    std::vector<PetakLahan*> getPropertiByGrup(std::string warna);

    PetakProperti* getProperti(int id);

    int hitungUtility(Pemain* pemain);

    int getPengaliUtility(int jumlah);

    int hitungRailroad(Pemain* pemain);
    
    int getHargaSewaRailroad(int jumlah);

    ~ManagerProperti();

    const std::map<int, PetakProperti*>& getDaftarProperti() { return daftarProperti; }
    int hitungJumlahProperti() { return daftarProperti.size(); }
};