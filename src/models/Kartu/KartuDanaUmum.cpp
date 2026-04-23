#include "models/Kartu/KartuDanaUmum.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include <iostream>

void KartuHariUlangTahun::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "Kartu: \"" << getNamaKartu() << "\"\n";
    std::cout << "Kamu mendapatkan M" << JUMLAH << " dari setiap pemain lain.\n";
    svc.collectFromAllPlayers(pemain, JUMLAH);
}

void KartuBiayaDokter::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "Kartu: \"" << getNamaKartu() << "\"\n";
    std::cout << "Kamu membayar M" << JUMLAH << " ke Bank.\n";
    svc.transferMoney(&pemain, nullptr, JUMLAH);
}

void KartuMauNyaleg::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "Kartu: \"" << getNamaKartu() << "\"\n";
    std::cout << "Kamu membayar M" << JUMLAH << " kepada setiap pemain lain.\n";
    svc.payToAllPlayers(pemain, JUMLAH);
}