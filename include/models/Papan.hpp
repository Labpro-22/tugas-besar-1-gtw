#pragma once
#include "Petak/Petak.hpp"
#include "Petak/PetakProperti.hpp"
#include "Petak/PetakAksi.hpp"
#include "Petak/PetakSpesial.hpp"
#include "PlayerActionService.hpp"
#include "Properti/ManagerProperti.hpp"
#include <vector>

struct PetakInit {
    int indeks;
    std::string kode;
    std::string nama;
    std::string tipe;
};

class Papan{
private:
    std::vector<Petak*> daftarPetak;

public:
    Papan(ManagerProperti& manager); 

    Petak* getPetak(int indeks);

    Petak* getPetak(const std::string& kode);

    Petak* findNearestByKode(const std::string& kode, int indeks);

    int getTotalPetak();

    ~Papan();
};