#pragma once
#include <string>
#include <vector>
#include "ConfigData.hpp"
#include "Dadu.hpp"
#include "../core/BangkrutDanEndGame.hpp"

class Pemain;
class PetakLahan;
class PetakProperti;
class MovementController;
class Papan;
class ManagerPenjara;
class ManagerProperti;
class ManagerFestival;
class ManagerTransaksi;
class LogTransaksiGame;

class PlayerActionService {
private:
    MovementController* movementController;
    ManagerPenjara* managerPenjara;
    ManagerProperti* managerProperti;
    ManagerFestival* managerFestival;
    ManagerTransaksi* managerTransaksi;
    ConfigData* config;
    Papan* papan;
    LogTransaksiGame* logger;

    std::vector<Pemain*>* daftarPemain;
    bool arahNormal; // arah giliran bermain, kalau true = normal, false = reverse
    int* currentTurnIdx; // turn index saat ini untuk reverse

public:
    PlayerActionService(
        MovementController* mc,
        ManagerPenjara* mp,
        ManagerProperti* mpr,
        ManagerFestival* mf,
        ManagerTransaksi* mt,
        ConfigData* config,
        Papan* papan,
        LogTransaksiGame* log,
        std::vector<Pemain*>* pemain,
        int* turnIdx
    );
    ~PlayerActionService() = default;
    void movePlayerRelative(Pemain& p, int n); // untuk maju dan mundur sejumlah n petak, kalau negatif berarti mundur
    void teleportByInput(Pemain& p, const std::string& kodeOrIndeks); // teleport ke petak berdasarkan input
    void moveToNearestStation(Pemain& p); // pindah ke stasiun terdekat, dipakai KartuPergiKeStasiunTerdekat
    void sendToJail(Pemain& p); // kirim pemain ke penjara
    void escapeFromJailByCard(Pemain& p); // bebaskan pemain dari penjara pakai kartu
    void jailOpponent(Pemain& p); // untuk kartu bonus PenjaraKanCard, bisa menampilkan daftar pemain aktif selain diri sendiri lalu pilih yang akan dipenjarakan
    void transferMoney(Pemain* asal, Pemain* tujuan, int jumlah); // nullptr = ke/dari Bank
    void collectFromAllPlayers(Pemain& penerima, int jumlahPerOrang); // untuk KartuHariUlangTahun
    void payToAllPlayers(Pemain& pembayar, int jumlahPerOrang); // untuk KartuMauNyaleg

    /** Hitung dan eksekusi pembayaran sewa. */
    void bayarSewa(Pemain& penyewa, PetakProperti& properti, int nilaiDadu);
    /** Eksekusi pembelian properti oleh pemain. */
    void beliProperti(Pemain& pemain, PetakProperti& properti);
    /** Eksekusi gadai properti ke bank. */
    void gadaiProperti(Pemain& pemain, PetakProperti& properti);
    /** Eksekusi bangun rumah/hotel pada properti street. */
    void bangunProperti(Pemain& pemain, PetakLahan& properti);

    void demolishOpponentProperty(Pemain& pemain); // untuk DemolitionCard
    void pullPlayerAhead(Pemain& pemain); // untuk LassoCard
    void rotateAllHandCards(Pemain& pemain); // untuk bonus RotasiKartuCard
    void reverseTurnOrder(Pemain& pemain); // untuk bonus ReverseCard

    /* Buat Kebangkrutan */
    void beriSemuaAset (Pemain* asal, Pemain *tujuan); // nullptr = bank

    void logFestivalActivation(Pemain& pemain, PetakProperti& prop, int pengali, int durasi);
    void logAksi(Pemain& pemain, const std::string& aksi, const std::string& detail);
    bool isArahNormal() const { return arahNormal; }
    const std::vector<Pemain*>& getDaftarPemain() const { return *daftarPemain; }

    ConfigData* getConfig() { return config; }
};
