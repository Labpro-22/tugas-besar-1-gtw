#include "../../../include/models/Petak/PetakPajak.hpp"
#include "../../../include/models/Pemain.hpp"
#include "../../../include/models/PlayerActionService.hpp"
#include "../../../include/core/BangkrutDanEndGame.hpp"

PetakPajak::PetakPajak(int indeks, const std::string& kode, const std::string& nama, ConfigData* konfigurasi)
    : PetakAksi(indeks, kode, nama), config(konfigurasi) {
}

// Override method: onLanded
void PetakPajak::onLanded(Pemain& pemain, PlayerActionService& svc) {
    if (jenis == JenisPajak::PAJAK_BARANG_MEWAH) {
        svc.transferMoney(&pemain, nullptr, config->getPbmFlat());
    }
    else {
        cout << "Kamu mendarat di Pajak Penghasilan (PPH)!\n";
        cout << "Pilih opsi pembayaran pajak: \n";
        cout << "1. Bayar flat M" << config->getPphFlat() << " \n";
        cout << "2. Bayar " << config->getPphPersen() << "\% dari total kekayaan\n";
        cout << "(Pilih sebelum menghitung kekayaan!\n)";

        string input;
        while (true) {
            cout << "Pilihan(1/2): ";
            getline(cin, input);
            if (input != "1" && input != "2") {
                cout << "Masukan tidak valid\n";
            }
        }

        if (input == "1") {
            svc.transferMoney (&pemain, nullptr, config->getPphFlat());
        }
        else {
            int harusDibayar = 1.0*Likuidasi::totalLikuidasiSeluruhAsetJual(&pemain)*config->getPphPersen() / 100;
            svc.transferMoney (&pemain, nullptr, harusDibayar);
        }
    }
}
