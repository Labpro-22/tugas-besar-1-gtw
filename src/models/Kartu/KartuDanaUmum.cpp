#include "models/Kartu/KartuDanaUmum.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "utils/NimonspoliException.hpp"
#include <iostream>

void KartuHariUlangTahun::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "Kartu: \"" << getNamaKartu() << "\"\n";
    std::cout << "Kamu mendapatkan M" << JUMLAH << " dari setiap pemain lain.\n";
    svc.collectFromAllPlayers(pemain, JUMLAH);
}

void KartuBiayaDokter::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "Kartu: \"" << getNamaKartu() << "\"\n";
    try {
        svc.transferMoney(&pemain, nullptr, JUMLAH);
        std::cout << "Kamu membayar M" << JUMLAH << " ke Bank.\n";
    } catch (const UangTidakCukupException&) {
        std::cout << "[!] Uang tidak cukup untuk membayar biaya dokter (M" << JUMLAH << ").\n";
        int sisa = pemain.getSaldo();
        svc.transferMoney(&pemain, nullptr, sisa);
        std::cout << " Membayar sisa saldo: M" << sisa << ".\n";
    }
}

void KartuMauNyaleg::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "Kartu: \"" << getNamaKartu() << "\"\n";
    try {
        svc.payToAllPlayers(pemain, JUMLAH);
        std::cout << "Kamu membayar M" << JUMLAH << " kepada setiap pemain lain.\n";
    } catch (const UangTidakCukupException&) {
        std::cout << " Uang tidak cukup untuk membayar M" << JUMLAH << " kepada semua pemain.\n";
        std::cout << " Saldo tersisa: M" << pemain.getSaldo() << ".\n";
        throw;
    }
}