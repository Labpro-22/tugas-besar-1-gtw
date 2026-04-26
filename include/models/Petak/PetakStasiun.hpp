#pragma once
#include "PetakProperti.hpp"

class PetakStasiun : public PetakProperti {
public:
    PetakStasiun(int indeks, const std::string& kode, const std::string& nama, const PropertiConfig& config);
    int hitungSewa(int dadu, ManagerProperti& manager) override;
    std::string getJenisString() const override;
    ~PetakStasiun() = default;
};
