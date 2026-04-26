#pragma once
#include "PetakProperti.hpp"
#include <vector>

class PetakLahan : public PetakProperti {
public:
    enum class ColorGroup {
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

    PetakLahan(int indeks, const std::string& kode, const std::string& nama, const PropertiConfig& config);
    
    bool punyaHotel() const;
    int hitungSewa(int dadu, ManagerProperti& manager) override;
    void bangun(ManagerProperti& manager); // maks 4 rumah + 1 hotel
    int getJumlahBangunan() const;
    int getHargaBangun() const;
    std::string getJenisString() const override;
    std::string getBangunanString() const override;
    std::string getJumlahBangunanString() const;
    ColorGroup getWarna() const;
    std::string getWarnaString() const;
    void turunkanBangunan();
    static ColorGroup convertWarna(const std::string& warnaStr);
    
    ~PetakLahan() = default;

private:
    ColorGroup warna;
    int level; // 0 : kosong - 5 : punya hotel dan 4 rumah 
    std::vector<int> tabelHargaSewa;
};
