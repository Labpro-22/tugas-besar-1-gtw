#pragma once

#include <vector>

class Pemain;
class LogTransaksiGame;

class ManagerTransaksi {
private:
    LogTransaksiGame* logger;
    std::vector<Pemain*>* daftarPemain;

public:
    ManagerTransaksi(LogTransaksiGame* log, std::vector<Pemain*>* pemain);
    ~ManagerTransaksi() = default;

    void transferMoney(Pemain* asal, Pemain* tujuan, int jumlah);
    void collectFromAllPlayers(Pemain& penerima, int jumlahPerOrang);
    void payToAllPlayers(Pemain& pembayar, int jumlahPerOrang);
    void beriSemuaAset(Pemain* asal, Pemain* tujuan);
};
