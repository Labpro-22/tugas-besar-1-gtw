#pragma once
#include <map>
#include <utility>
#include <string>

class PetakProperti;
class Pemain;

class ManagerFestival {
private:
    std::map<PetakProperti*, std::pair<int, int>> efekAktif;
    static constexpr int PENGALI_MAKS = 8;
    static constexpr int DURASI_DEFAULT = 3;
public:
    void applyFestival(PetakProperti* properti);
    void updateDurasi(Pemain* pemilik);
    int getPengali(PetakProperti* properti) const;
    int getSisaDurasi(PetakProperti* properti) const;
    bool isAktif(PetakProperti* properti) const;
    std::string getFestivalString(PetakProperti* properti) const;
    void setEfekFestival(PetakProperti* properti, int pengali, int durasi);
};
