#pragma once
#include "Petak.hpp"

class PetakAksi : public Petak{
public:
    PetakAksi(int indeks, const std::string& kode, const std::string& nama);

    virtual void onLanded(Pemain& pemain, PlayerActionService& actionService) = 0;

    virtual ~PetakAksi() = default;
};