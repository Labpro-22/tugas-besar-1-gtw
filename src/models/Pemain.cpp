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
void Pemain::tambahKartu(KartuKemampuanSpesial* kartuBaru, DeckKartu<KartuKemampuanSpesial>& deckSkill){
    kartuDiTangan.push_back(kartuBaru);
    if (kartuDiTangan.size() <= 3) {
        cout << username << " mendapatkan kartu: " << kartuBaru->getDeskripsi() << "\n";
        return;
    }
    cout << "PERINGATAN: Kamu sudah memiliki 3 kartu di tangan (maksimal 3).\n";
    cout << "Kamu wajib membuang 1 kartu.\n";
    cout << "Daftar Kartu Kemampuan Anda:\n";
    for (size_t i = 0; i < kartuDiTangan.size(); ++i) {
        cout << (i + 1) << ". " << kartuDiTangan[i]->getDeskripsi() << "\n";
    }
    int pilihan = -1;
    while (true) {
        cout << "Pilih nomor kartu yang ingin dibuang (1-" << kartuDiTangan.size() << "): ";
        if (cin >> pilihan && pilihan >= 1 && pilihan <= static_cast<int>(kartuDiTangan.size())) {
            break;
        }
        cout << "Input tidak valid. Coba lagi.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    KartuKemampuanSpesial* kartuDibuang = kartuDiTangan[pilihan - 1];
    deckSkill.buangKartu(kartuDibuang);
    kartuDiTangan.erase(kartuDiTangan.begin() + (pilihan - 1));

    cout << kartuDibuang->getNamaKartu() << " telah dibuang. Sekarang kamu memiliki " << kartuDiTangan.size() << " kartu.\n";
} 

void Pemain::buangKartu(int index, DeckKartu<KartuKemampuanSpesial>& deckSkill){
    assert(index >= 0 && index < static_cast<int>(kartuDiTangan.size()));
    KartuKemampuanSpesial* kartu = kartuDiTangan[index];
    deckSkill.buangKartu(kartu);
    kartuDiTangan.erase(kartuDiTangan.begin() + index);
}

void Pemain::gunakanKartu(int index, PlayerActionService& svc, DeckKartu<KartuKemampuanSpesial>& deckSkill, bool bolehPakaiSkill){
    assert(index >= 0 && index < static_cast<int>(kartuDiTangan.size()));
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