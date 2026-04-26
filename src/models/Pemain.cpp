#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "utils/NimonspoliException.hpp"
#include <iostream>
#include <limits>
#include <stdexcept>

using namespace std;

Pemain::Pemain(const std::string& username, int saldoAwal) : username(username), posisi(0), saldo(saldoAwal), status(StatusPemain::ACTIVE), shieldAktif(false), discountAktif(false), persenDiskon(0), sudahPakaiSkill(false), percobaanKeluarPenjara(0), punyaKartuBebasPenjara(false) {};
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

void Pemain::tambahKartu(KartuKemampuanSpesial* kartuBaru, DeckKartu<KartuKemampuanSpesial>& deckSkill){
    if (kartuDiTangan.size() >= 3) {
        throw SlotKartuPenuhException();
    }
    kartuDiTangan.push_back(kartuBaru);
    cout << username << " mendapatkan kartu: " << kartuBaru->getDeskripsi() << "\n";
}

void Pemain::buangKartu(int index, DeckKartu<KartuKemampuanSpesial>& deckSkill){
    if (index < 0 || index >= static_cast<int>(kartuDiTangan.size())) throw std::out_of_range("buangKartu: index kartu tidak valid.");
    KartuKemampuanSpesial* kartu = kartuDiTangan[index];
    deckSkill.buangKartu(kartu);
    kartuDiTangan.erase(kartuDiTangan.begin() + index);
}

void Pemain::gunakanKartu(int index, PlayerActionService& svc, DeckKartu<KartuKemampuanSpesial>& deckSkill, bool bolehPakaiSkill){
    if (index < 0 || index >= static_cast<int>(kartuDiTangan.size())) throw std::out_of_range("gunakanKartu: index kartu tidak valid.");
    if (!bolehPakaiSkill) {
        cout << "Kartu kemampuan hanya bisa digunakan sebelum melempar dadu.\n";
        return;
    }
    if (sudahPakaiSkill) {
        cout << "Kamu sudah menggunakan kartu kemampuan pada giliran ini!\n";
        return;
    }
    KartuKemampuanSpesial* kartu = kartuDiTangan[index];
    kartu->gunakan(*this, svc);
    sudahPakaiSkill = true;
    deckSkill.buangKartu(kartu);
    kartuDiTangan.erase(kartuDiTangan.begin() + index);
    cout << kartu->getNamaKartu() << " telah digunakan dan dipindahkan ke tumpukan pembuangan.\n";
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