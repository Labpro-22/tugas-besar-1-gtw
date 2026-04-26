#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "utils/NimonspoliException.hpp"
#include <iostream>
#include <limits>
#include <cassert>

using namespace std;

Pemain::Pemain(const std::string& username, int saldoAwal) : username(username), posisi(0), saldo(saldoAwal), status(StatusPemain::ACTIVE), shieldAktif(false), discountAktif(false), persenDiskon(0), sudahPakaiSkill(false), percobaanKeluarPenjara(0) {};
Pemain::~Pemain() {};

// operator overload 
Pemain& Pemain::operator+=(int amount){
    saldo += amount;
    return *this;
}
Pemain& Pemain::operator-=(int amount){
    if (saldo < amount) throw UangTidakCukupException();
    
    saldo -= amount;
    return *this;
}
bool Pemain::operator>(const Pemain& other) const{
    //  Pemenang adalah pemain dengan uang terbanyak. 
    //  Jika seri, pemain yang memiliki jumlah petak properti terbanyak yang menjadi pemenang. 
    //  Jika seri, pemain yang memiliki jumlah kartu terbanyak yang menjadi pemenang. 
    //  Jika seri, semua pemain yang masih seri menjadi pemenang.
    if (saldo != other.saldo) return saldo > other.saldo;
    if (asetPemain.size() != other.asetPemain.size()) return asetPemain.size() > other.asetPemain.size();
    if (kartuDiTangan.size() != other.kartuDiTangan.size()) return kartuDiTangan.size() > other.kartuDiTangan.size();
    return false;
}
bool Pemain::operator<(const Pemain& other) const{ 
    if (saldo != other.saldo) return saldo < other.saldo;
    if (asetPemain.size() != other.asetPemain.size()) return asetPemain.size() < other.asetPemain.size();
    if (kartuDiTangan.size() != other.kartuDiTangan.size()) return kartuDiTangan.size() < other.kartuDiTangan.size();
    return false;
}

// kartu management 
void Pemain::tambahKartu(KartuKemampuanSpesial* kartuBaru, DeckKartu<KartuKemampuanSpesial>& /*deckSkill*/){
    if (kartuDiTangan.size() >= 3) {
        throw SlotKartuPenuhException();
    }
    kartuDiTangan.push_back(kartuBaru);
}

// properti management 
void Pemain::tambahAset(PetakProperti* p){
    asetPemain.push_back(p);
}
void Pemain::hapusAset(PetakProperti* p){
    for (auto it = asetPemain.begin(); it != asetPemain.end(); it++){
        if (*it == p){
            asetPemain.erase(it);
            return;
        }
    }
}
string Pemain::getStatusString() {
    switch (status) {
    case StatusPemain::ACTIVE:
        return "ACTIVE";
    case StatusPemain::JAILED:
        return "JAILED";
    case StatusPemain::BANKRUPT:
        return "BANKRUPT";
    default:
        return "ACTIVE";
    }
}