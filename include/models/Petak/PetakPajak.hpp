#pragma once
#include "PetakAksi.hpp"
#include "../ConfigData.hpp"

enum class JenisPajak { PAJAK_BARANG_MEWAH, PAJAK_PENGHASILAN };

class PetakPajak : public PetakAksi {
protected:
    JenisPajak jenis;
    ConfigData* config;

public:
    PetakPajak(int indeks, const std::string& kode, const std::string& nama, ConfigData* konfigurasi);
    ~PetakPajak() = default;
    void onLanded(Pemain& pemain, PlayerActionService& svc) override;

};