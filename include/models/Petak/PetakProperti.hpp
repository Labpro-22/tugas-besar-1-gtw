#pragma once
#include "Petak.hpp"
#include "models/Properti/Properti.hpp"
#include <string>

class PetakProperti : public Petak{
protected:
    Properti* properti;

public:
    PetakProperti(int indeks, const std::string& kode, const std::string& nama, Properti* properti);

    virtual void onLanded(Pemain& pemain, PlayerActionService& actionService);

    virtual ~PetakProperti() = default;
};