#include "models/Petak/PetakPajak.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "core/BangkrutDanEndGame.hpp"
#include "views/OutputHandler.hpp"
#include "views/InputHandler.hpp"

PetakPajak::PetakPajak(int indeks, const std::string& kode, const std::string& nama, ConfigData* konfigurasi)
    : PetakAksi(indeks, kode, nama), config(konfigurasi) {
    // Pastikan jenis pajak terdefinisi (sebelumnya uninitialized).
    // Mapping dari kode konfigurasi aksi.txt: "PPH" dan "PBM".
    if (kode == "PBM") {
        jenis = JenisPajak::PAJAK_BARANG_MEWAH;
    } else {
        jenis = JenisPajak::PAJAK_PENGHASILAN;
    }
}

// Override method: onLanded
void PetakPajak::onLanded(Pemain& pemain, PlayerActionService& svc) {
    if (jenis == JenisPajak::PAJAK_BARANG_MEWAH) {
        svc.transferMoney(&pemain, nullptr, config->getPbmFlat());
    }
    else {
        int pilihan = InputHandler::mintaPilihanPajak(config->getPphFlat(), config->getPphPersen());

        if (pilihan == 1) {
            svc.transferMoney (&pemain, nullptr, config->getPphFlat());
        }
        else {
            int harusDibayar = 1.0*Likuidasi::totalLikuidasiSeluruhAsetJual(&pemain)*config->getPphPersen() / 100;
            svc.transferMoney (&pemain, nullptr, harusDibayar);
        }
    }
}

