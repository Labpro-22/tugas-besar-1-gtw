#pragma once
#include "Petak/Petak.hpp"
#include "Petak/PetakProperti.hpp"
#include "Petak/PetakAksi.hpp"
#include "Petak/PetakSpesial.hpp"
#include "Managers/ManagerProperti.hpp"
#include "Kartu/DeckKartu.hpp"
#include "Kartu/Kartu.hpp"
#include <vector>

class ManagerFestival;

class Papan{
private:
    std::vector<Petak*> daftarPetak;

public:
    // dependency injection agar Petak Aksi tidak dibuat dengan nullptr
    Papan(
        ManagerProperti& manager,
        ConfigData& configData,
        ManagerFestival* managerFestival,
        DeckKartu<Kartu>* deckKesempatan,
        DeckKartu<Kartu>* deckDanaUmum
    ); 

    Petak* getPetak(int indeks);

    Petak* getPetak(const std::string& kode);

    Petak* findNearestByKode(const std::string& kode, int indeks);

    int getTotalPetak() const;
    const std::vector<Petak*>& getDaftarPetak() const { return daftarPetak; }

    ~Papan();
};