<<<<<<< HEAD
#include "models/Petak/PetakPajak.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "models/Petak/PetakLahan.hpp"
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
        OutputHandler::cetakPesan("Kamu mendarat di Pajak Barang Mewah (PBM)!");
        OutputHandler::cetakPesan("Pajak sebesar M" + std::to_string(config->getPbmFlat()) + " langsung dipotong.");
        svc.transferMoney(&pemain, nullptr, config->getPbmFlat());
    }
    else {
        int pilihan = InputHandler::mintaPilihanPajak(config->getPphFlat(), config->getPphPersen());

        if (pilihan == 1) {
            OutputHandler::cetakPesan("Kamu memilih bayar flat M" + std::to_string(config->getPphFlat()) + ".");
            svc.transferMoney (&pemain, nullptr, config->getPphFlat());
        }
        else {
            // Sesuai spesifikasi: total kekayaan = cash + harga beli properti + harga beli bangunan (penuh).
            long long totalKekayaan = pemain.getSaldo();
            for (PetakProperti* aset : pemain.getAsetPemain()) {
                if (!aset || !aset->getConfigProperti()) continue;

                totalKekayaan += aset->getConfigProperti()->getHargaLahan();

                if (auto lahan = dynamic_cast<PetakLahan*>(aset)) {
                    int level = lahan->getJumlahBangunan();
                    if (level <= 0) continue;

                    if (level >= 5) {
                        // Hotel: total biaya = 4 rumah + 1 hotel.
                        totalKekayaan += 4LL * lahan->getConfigProperti()->getHargaBangunRumah();
                        totalKekayaan += lahan->getConfigProperti()->getHargaBangunHotel();
                    } else {
                        totalKekayaan += 1LL * level * lahan->getConfigProperti()->getHargaBangunRumah();
                    }
                }
            }

            int harusDibayar = static_cast<int>((totalKekayaan * config->getPphPersen()) / 100);
            OutputHandler::cetakPesan("Total kekayaan kamu: M" + std::to_string(totalKekayaan));
            OutputHandler::cetakPesan("Pajak " + std::to_string(config->getPphPersen()) + "%: M" + std::to_string(harusDibayar));
            svc.transferMoney (&pemain, nullptr, harusDibayar);
        }
    }
}

=======
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

>>>>>>> 4c863580e8432dbd781567a56c63fe50b90b3f39
