#pragma once
#include <string>
#include <vector>
#include "models/Kartu/DeckKartu.hpp"
#include "models/Kartu/KartuKemampuanSpesial.hpp"

// Forward declaration
class KartuKemampuanSpesial; 
class PetakProperti;
class PlayerActionService; 

enum class StatusPemain { ACTIVE, JAILED, BANKRUPT };

class Pemain {
private:
    std::string username;
    int posisi;
    int saldo;
    StatusPemain status; 
    std::vector<KartuKemampuanSpesial*> kartuDiTangan;
    std::vector<PetakProperti*> asetPemain;
   
    bool shieldAktif;
    bool discountAktif;
    int  persenDiskon;
    bool sudahPakaiSkill; // reset tiap awal giliran
    int percobaanKeluarPenjara; // 0-3, reset saat bebas

public:
    Pemain(const std::string& username, int saldoAwal);
    ~Pemain();

    //operator overload 
    Pemain& operator+=(int amount);   // terima uang
    Pemain& operator-=(int amount);   // bayar uang
    bool operator>(const Pemain& other) const;  // bandingin kekayaan
    bool operator<(const Pemain& other) const;  // bandingin kekayaan

    //hanya set posisi pemain. Aturan move di papan handled by MovementController 
    void setPosisi(int pos) { posisi = pos; }

    //kartu management 
    void tambahKartu(KartuKemampuanSpesial* kartuBaru, DeckKartu<KartuKemampuanSpesial>& deckSkill);  
    void buangKartu(int index, DeckKartu<KartuKemampuanSpesial>& deckSkill);  //digunakan kalau kartuDiTangan penuh 
    void gunakanKartu(int index, PlayerActionService& svc, DeckKartu<KartuKemampuanSpesial>& deckSkill, bool bolehPakaiSkill); 

    //status management
    void setStatus(const StatusPemain& s) { status = s; }
    void setShieldAktif(bool val)        { shieldAktif = val; }
    void setDiscountAktif(bool val, int persen = 0) { discountAktif = val; persenDiskon = persen; }
    void resetPercobaanPenjara()         { percobaanKeluarPenjara = 0; }
    void tambahPercobaanPenjara()        { percobaanKeluarPenjara++; }
    void resetSkillUsage()               { sudahPakaiSkill = false; }  // dipanggil awal giliran

    //properti management. HitungAsetRailroad dan hitungAsetUtility handled by ManagerProperti
    void tambahAset(PetakProperti* p);
    void hapusAset(PetakProperti* p);

    //getter
    const std::string& getUsername()    const { return username; }
    int  getPosisi()                    const { return posisi; }
    int  getSaldo()                     const { return saldo; }
    const StatusPemain& getStatus()      const { return status; }
    bool isShieldAktif()                const { return shieldAktif; }
    bool isDiscountAktif()              const { return discountAktif; }
    bool isSudahPakaiSkill()            const { return sudahPakaiSkill; }
    int  getPersenDiskon()              const { return persenDiskon; }
    int  getPercobaanKeluarPenjara()    const { return percobaanKeluarPenjara; }
    const std::vector<KartuKemampuanSpesial*>& getKartuDiTangan() const { return kartuDiTangan; }
    const std::vector<PetakProperti*>&              getAsetPemain()    const { return asetPemain ; }

    void setKartuDiTangan(const std::vector<KartuKemampuanSpesial*>& kartu) {
        kartuDiTangan = kartu;
    }
    //helper
    std::string getStatusString();
};