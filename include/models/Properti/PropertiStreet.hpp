#pragma once
#include "Properti.hpp"

class PropertiStreet : public Properti{
public:
    enum class ColorGroup{
        DEFAULT,
        COKLAT,
        BIRU_MUDA,
        MERAH_MUDA,
        ABU_ABU,
        ORANGE,
        MERAH,
        KUNING,
        HIJAU,
        BIRU_TUA
    };

    PropertiStreet(const PropertiConfig& config);
    bool punyaHotel() const;
    int hitungSewa(int dadu, ManagerProperti& manager) override;
    void bangun(ManagerProperti& manager); //maks 4 rumah + 1 hotel
    int getJumlahBangunan() const;
    int getHargaBangun() const;
    ColorGroup getWarna() const;
    void turunkanBangunan();
    ColorGroup convertWarna(const std::string& warnaStr);
    ~PropertiStreet() = default;

private:
    ColorGroup warna;
    int level; // 0 : kosong - 5 : punya hotel dan 4 rumah 
    std::vector<int> tabelHargaSewa;
};
