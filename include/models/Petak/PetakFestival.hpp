#pragma once
#include "PetakAksi.hpp"
#include <map>
#include <utility>
#include <string>

class Properti;
class Pemain;
class PlayerActionService;

class ManagerFestival;

class PetakFestival : public PetakAksi {
private:
    ManagerFestival* managerFestival;
public:
    PetakFestival(int indeks, const std::string& kode, const std::string& nama, ManagerFestival* mgr);
    void onLanded(Pemain& pemain, PlayerActionService& svc) override;
};