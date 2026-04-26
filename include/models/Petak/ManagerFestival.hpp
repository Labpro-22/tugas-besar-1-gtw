#pragma once
#include <map>
#include <utility>

class Properti;
class Pemain;

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