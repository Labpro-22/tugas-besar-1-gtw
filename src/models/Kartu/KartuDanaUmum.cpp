#include "views/OutputHandler.hpp"
#include "models/Kartu/KartuDanaUmum.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include <iostream>

void KartuHariUlangTahun::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    
    OutputHandler::cetakEfekKartu(getNamaKartu(), "Kamu mendapatkan M" + std::to_string(JUMLAH) + " dari setiap pemain lain.");
    svc.collectFromAllPlayers(pemain, JUMLAH);
}

void KartuBiayaDokter::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    
    OutputHandler::cetakEfekKartu(getNamaKartu(), "Kamu membayar M" + std::to_string(JUMLAH) + " ke Bank.");
    svc.transferMoney(&pemain, nullptr, JUMLAH);
}

void KartuMauNyaleg::jalankanEfek(Pemain& pemain, PlayerActionService& svc) {
    
    OutputHandler::cetakEfekKartu(getNamaKartu(), "Kamu membayar M" + std::to_string(JUMLAH) + " kepada setiap pemain lain.");
    svc.payToAllPlayers(pemain, JUMLAH);
}