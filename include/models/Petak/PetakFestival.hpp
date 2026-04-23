#pragma once
#include "PetakAksi.hpp"
#include <map>
#include <utility>
#include <string>

class Properti;
class Pemain;
class PlayerActionService;

class ManagerFestival {
private:
    std::map<Properti*, std::pair<int, int>> efekAktif;
    static constexpr int PENGALI_MAKS = 8;
    static constexpr int DURASI_DEFAULT = 3;
public:
    void applyFestival(Properti* properti);
    void updateDurasi(Pemain* pemilik);
    int getPengali(Properti* properti) const;
    int getSisaDurasi(Properti* properti) const;
    bool isAktif(Properti* properti) const;
};

class PetakFestival : public PetakAksi {
private:
    ManagerFestival* managerFestival;
public:
    PetakFestival(int indeks, const std::string& kode, const std::string& nama, ManagerFestival* mgr);
    void onLanded(Pemain& pemain, PlayerActionService& svc) override;
};