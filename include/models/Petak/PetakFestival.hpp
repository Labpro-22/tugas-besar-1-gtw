#pragma once
#include "PetakAksi.hpp"
#include "models/Petak/ManagerFestival.hpp"

class Pemain;
class PlayerActionService;

class PetakFestival : public PetakAksi {
private:
    ManagerFestival* managerFestival;
public:
    PetakFestival(int indeks, const std::string& kode, const std::string& nama, ManagerFestival* mgr);
    void onLanded(Pemain& pemain, PlayerActionService& svc) override;
};