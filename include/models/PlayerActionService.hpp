#pragma once
#include <string>
#include <vector>
#include "models/Petak/ManagerFestival.hpp"

class Pemain;
class PropertiStreet;
class Properti;
class MovementController;
class ManagerPenjara;
class ManagerProperti;
class LogTransaksiGame;

class PlayerActionService {
private:
    MovementController* movementController;
    ManagerPenjara* managerPenjara;
    ManagerProperti* managerProperti;
    ManagerFestival* managerFestival;
    LogTransaksiGame* logger;

    std::vector<Pemain*>* daftarPemain;
    bool arahNormal; // arah giliran bermain, kalau true = normal, false = reverse
    int* currentTurnIdx; // turn index saat ini untuk reverse

public:
    PlayerActionService(MovementController* mc, ManagerPenjara* mp, ManagerProperti* mpr, ManagerFestival* mf, LogTransaksiGame* log, std::vector<Pemain*>* pemain, int* turnIdx);
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
    void bayarSewa(Pemain& penyewa, Properti& properti, int nilaiDadu);
    /** Eksekusi pembelian properti oleh pemain. */
    void beliProperti(Pemain& pemain, Properti& properti);
    /** Eksekusi gadai properti ke bank. */
    void gadaiProperti(Pemain& pemain, Properti& properti);
    /** Eksekusi bangun rumah/hotel pada properti street. */
    void bangunProperti(Pemain& pemain, PropertiStreet& properti);

    void simpanKartuBebasPenjara(Pemain& pemain, Kartu* kartu);
    void demolishOpponentProperty(Pemain& pemain); // untuk DemolitionCard
    void pullPlayerAhead(Pemain& pemain); // untuk LassoCard
    void rotateAllHandCards(Pemain& pemain); // untuk bonus RotasiKartuCard
    void reverseTurnOrder(Pemain& pemain); // untuk bonus ReverseCard

    void logFestivalActivation(Pemain& pemain, Properti& prop, int pengali, int durasi);
    void logAksi(Pemain& pemain, const std::string& aksi, const std::string& detail);
    bool isArahNormal() const { return arahNormal; }
    const std::vector<Pemain*>& getDaftarPemain() const { return *daftarPemain; }
};
