#pragma once
#include "Petak/Petak.hpp"
#include "Petak/PetakProperti.hpp"
#include "Petak/PetakAksi.hpp"
#include "Petak/PetakSpesial.hpp"
#include "PlayerActionService.hpp"
#include "Managers/ManagerProperti.hpp"
#include <vector>

class Papan{
private:
    std::vector<Petak*> daftarPetak;

public:
    Papan(ManagerProperti& manager, ConfigData& configData); 

    Petak* getPetak(int indeks);

    Petak* getPetak(const std::string& kode);

    Petak* findNearestByKode(const std::string& kode, int indeks);

    int getTotalPetak() const;

    ~Papan();
};