#pragma once
#include "Properti.hpp"
#include "PropertiStreet.hpp"
#include "PropertiRailroad.hpp"
#include "PropertiUtility.hpp"
#include "models/ConfigData.hpp"
#include <vector>
#include <string>

class ManagerProperti{
protected:
    std::map<std::string, Properti*> daftarProperti;
    const ConfigData& configData;
    
public:
    ManagerProperti(const ConfigData& configData);

    bool isMonopoly(Pemain* pemain, PropertiStreet::ColorGroup warna);

    bool isBisaBangun(PropertiStreet* target); // level < 5 dan selisih jumlah rumah antar petak tidak lebih dari 1

    void jualSemuaBangunan(Pemain* pemain, PropertiStreet::ColorGroup warna);

    int hitungLikuidasi(Pemain* pemain);

    bool isPunyaSemuaDalamGrup(Pemain* pemain, PropertiStreet::ColorGroup warna);

    std::vector<PropertiStreet*> getPropertiByGrup(PropertiStreet::ColorGroup warna);

    Properti* getProperti(const std::string& kode);

    int hitungUtility(Pemain* pemain);

    int getPengaliUtility(int jumlah);

    int hitungRailroad(Pemain* pemain);
    
    int getHargaSewaRailroad(int jumlah);

    ~ManagerProperti();
};