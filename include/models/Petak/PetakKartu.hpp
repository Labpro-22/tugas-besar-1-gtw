#pragma once
#include "PetakAksi.hpp"
#include "../Kartu/DeckKartu.hpp"
#include "../Kartu/Kartu.hpp"

class PetakKartu : public PetakAksi {
public:
    enum class JenisKartu { KESEMPATAN, DANA_UMUM };
private:
    JenisKartu jenis;
    DeckKartu<Kartu>* deck;
public:
    PetakKartu(int indeks, const std::string& kode, const std::string& nama, JenisKartu jenis, DeckKartu<Kartu>* deck);
    ~PetakKartu() = default;
    void onLanded(Pemain& pemain, PlayerActionService& svc) override;
    JenisKartu getJenis() const { return jenis; }
};