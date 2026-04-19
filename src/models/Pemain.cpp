#include "models/Pemain.hpp"
// #include "models/KartuKemampuanSpesial.hpp"
#include "utils/NimonspoliException.hpp"
#include <cassert>

using namespace std;

Pemain::Pemain(const std::string& username, int saldoAwal) : username(username), saldo(saldoAwal) {};
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
    return true;
}
bool Pemain::operator<(const Pemain& other) const{ 
    if (saldo != other.saldo) return saldo < other.saldo;
    if (asetPemain.size() != other.asetPemain.size()) return asetPemain.size() < other.asetPemain.size();
    if (kartuDiTangan.size() != other.kartuDiTangan.size()) return kartuDiTangan.size() < other.kartuDiTangan.size();
    return true;
}

// kartu management 
void Pemain::tambahKartu(KartuKemampuanSpesial* kartu){
    if (kartuDiTangan.size() == 3){
        throw SlotKartuPenuhException();
    }
    kartuDiTangan.push_back(kartu);
}  
void Pemain::buangKartu(int index){
    assert(index >= 0 && index < kartuDiTangan.size());

    kartuDiTangan.erase(kartuDiTangan.begin() + index);
}
void Pemain::gunakanKartu(int index){
    assert(index >= 0 && index < kartuDiTangan.size());
    sudahPakaiSkill = true;
    // TODO: jalankan efek dari KartuKemampuanSpesial.hpp
    // kartuDiTangan[index]->gunakan(*this);
    buangKartu(index);
}

// properti management 
void Pemain::tambahAset(Properti* p){
    asetPemain.push_back(p);
}
void Pemain::hapusAset(Properti* p){
    for (auto it = asetPemain.begin(); it != asetPemain.end(); it++){
        if (*it == p){
            asetPemain.erase(it);
            return;
        }
    }
}