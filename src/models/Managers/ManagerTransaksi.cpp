<<<<<<< HEAD
#include "views/OutputHandler.hpp"
#include "models/Managers/ManagerTransaksi.hpp"
#include "models/Pemain.hpp"
#include "core/BangkrutDanEndGame.hpp"
#include "utils/LogTransaksiGame.hpp"
#include "utils/NimonspoliException.hpp"
#include "models/Petak/PetakLahan.hpp"
#include "models/Managers/ManagerLelang.hpp"
#include <iostream>

ManagerTransaksi::ManagerTransaksi(LogTransaksiGame* log, std::vector<Pemain*>* pemain) 
    : logger(log), daftarPemain(pemain) {}

void ManagerTransaksi::beriSemuaAset(Pemain* asal, Pemain* tujuan) {
    std::vector<PetakProperti*> asetBakalDipindah = asal->getAsetPemain();
    if (tujuan) {
        int saldoAsal = asal->getSaldo();
        if (saldoAsal > 0) {
            *tujuan += saldoAsal;
            *asal -= saldoAsal;
        }
        for (auto aset : asetBakalDipindah) {
            PetakProperti::StatusProperti statusLama = aset->getStatus();
            asal->hapusAset(aset);
            aset->setPemilik(tujuan);
            // Sesuai spesifikasi: aset tergadai tetap tergadai saat dialihkan ke kreditor pemain.
            aset->setStatus(statusLama);
            tujuan->tambahAset(aset);
        }
    } else {
        int saldoAsal = asal->getSaldo();
        if (saldoAsal > 0) {
            // Kas sisa pemain bangkrut ke Bank hilang dari peredaran.
            *asal -= saldoAsal;
        }
        ManagerLelang lelang(this);
        for (auto aset : asetBakalDipindah) {
            asal->hapusAset(aset);
            aset->setPemilik(nullptr);
            aset->setStatus(PetakProperti::StatusProperti::BANK);
            
            PetakLahan* street = dynamic_cast<PetakLahan*>(aset);
            if (street) {
                while (street->getJumlahBangunan() > 0) {
                    street->turunkanBangunan();
                }
            }
            lelang.mulaiLelang(aset, *daftarPemain, asal);
        }
    }
}

void ManagerTransaksi::transferMoney(Pemain* asal, Pemain* tujuan, int jumlah) {
    if (asal) {
        if (asal->getSaldo() < jumlah) {
            // Coba bayar dengan likuidasi
            if (Likuidasi::bisaBayarDenganLikuidasi(asal, jumlah)) {
                Likuidasi::tampilkanPanelLikuidasi(asal, jumlah);
                // Jika masih tidak cukup setelah likuidasi (mungkin karena dibatalkan pemain), throw
                if (asal->getSaldo() < jumlah) {
                    throw UangTidakCukupException();
                }
                *asal -= jumlah;
            } else {
                beriSemuaAset(asal, tujuan);
                Kebangkrutan::declareBangkrut(asal);
                throw NimonspoliException(101, "Pemain Bangkrut!"); // Throw custom exception agar game loop tau
            }
        } else {
            *asal -= jumlah;
        }
    } 
    if (tujuan) *tujuan += jumlah;
}

void ManagerTransaksi::collectFromAllPlayers(Pemain& penerima, int jumlahPerOrang) {
    for (Pemain* p : *daftarPemain) {
        if (p == &penerima) continue;
        if (p->getStatus() != StatusPemain::ACTIVE) continue;
        try {
            transferMoney(p, &penerima, jumlahPerOrang);
            OutputHandler::cetakTransaksi(p->getUsername(), jumlahPerOrang, penerima.getUsername());
        } catch (UangTidakCukupException&) {
            int sisa = p->getSaldo();
            transferMoney(p, &penerima, sisa);
            OutputHandler::cetakPesan(p->getUsername() + " hanya bisa membayar M" + std::to_string(sisa));
        } catch (NimonspoliException&) {
            // Sudah bangkrut, lewatkan
        }
    }
}

void ManagerTransaksi::payToAllPlayers(Pemain& pembayar, int jumlahPerOrang) {
    for (Pemain* p : *daftarPemain) {
        if (p == &pembayar) continue;
        if (p->getStatus() != StatusPemain::ACTIVE) continue;
        try {
            transferMoney(&pembayar, p, jumlahPerOrang);
            OutputHandler::cetakTransaksi(pembayar.getUsername(), jumlahPerOrang, p->getUsername());
        } catch (UangTidakCukupException&) {
            throw; // Teruskan exception agar game loop tahu
        } catch (NimonspoliException&) {
            throw;
        }
    }
}
=======
#include "views/OutputHandler.hpp"
#include "models/Managers/ManagerTransaksi.hpp"
#include "models/Pemain.hpp"
#include "core/BangkrutDanEndGame.hpp"
#include "utils/LogTransaksiGame.hpp"
#include "utils/NimonspoliException.hpp"
#include <iostream>

ManagerTransaksi::ManagerTransaksi(LogTransaksiGame* log, std::vector<Pemain*>* pemain) 
    : logger(log), daftarPemain(pemain) {}

void ManagerTransaksi::beriSemuaAset(Pemain* asal, Pemain* tujuan) {
    if (tujuan) {
        *tujuan += asal->getSaldo();
        for (auto aset : asal->getAsetPemain()) {
            aset->setPemilik(tujuan);
        }
    } else {
        for (auto aset : asal->getAsetPemain()) {
            aset->setPemilik(nullptr);
            aset->setStatus(PetakProperti::StatusProperti::BANK);
        }
    }
}

void ManagerTransaksi::transferMoney(Pemain* asal, Pemain* tujuan, int jumlah) {
    if (asal) {
        if (asal->getSaldo() < jumlah) {
            // Coba bayar dengan likuidasi
            if (Likuidasi::bisaBayarDenganLikuidasi(asal, jumlah)) {
                Likuidasi::tampilkanPanelLikuidasi(asal, jumlah);
                // Jika masih tidak cukup setelah likuidasi (mungkin karena dibatalkan pemain), throw
                if (asal->getSaldo() < jumlah) {
                    throw UangTidakCukupException();
                }
                *asal -= jumlah;
            } else {
                beriSemuaAset(asal, tujuan);
                Kebangkrutan::declareBangkrut(asal);
                throw NimonspoliException(101, "Pemain Bangkrut!"); // Throw custom exception agar game loop tau
            }
        } else {
            *asal -= jumlah;
        }
    } 
    if (tujuan) *tujuan += jumlah;
}

void ManagerTransaksi::collectFromAllPlayers(Pemain& penerima, int jumlahPerOrang) {
    for (Pemain* p : *daftarPemain) {
        if (p == &penerima) continue;
        if (p->getStatus() != StatusPemain::ACTIVE) continue;
        try {
            transferMoney(p, &penerima, jumlahPerOrang);
            OutputHandler::cetakTransaksi(p->getUsername(), jumlahPerOrang, penerima.getUsername());
        } catch (UangTidakCukupException&) {
            int sisa = p->getSaldo();
            transferMoney(p, &penerima, sisa);
            OutputHandler::cetakPesan(p->getUsername() + " hanya bisa membayar M" + std::to_string(sisa));
        } catch (NimonspoliException&) {
            // Sudah bangkrut, lewatkan
        }
    }
}

void ManagerTransaksi::payToAllPlayers(Pemain& pembayar, int jumlahPerOrang) {
    for (Pemain* p : *daftarPemain) {
        if (p == &pembayar) continue;
        if (p->getStatus() != StatusPemain::ACTIVE) continue;
        try {
            transferMoney(&pembayar, p, jumlahPerOrang);
            OutputHandler::cetakTransaksi(pembayar.getUsername(), jumlahPerOrang, p->getUsername());
        } catch (UangTidakCukupException&) {
            throw; // Teruskan exception agar game loop tahu
        } catch (NimonspoliException&) {
            throw;
        }
    }
}
>>>>>>> 4c863580e8432dbd781567a56c63fe50b90b3f39
