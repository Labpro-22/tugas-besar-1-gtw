#include "models/Managers/ManagerTransaksi.hpp"
#include "models/PlayerActionService.hpp"
#include "models/Managers/ManagerProperti.hpp"
#include "models/Pemain.hpp"
#include "models/Petak/PetakProperti.hpp"
#include "models/Papan.hpp"
#include "views/InputHandler.hpp"
#include "views/OutputHandler.hpp"
#include "utils/LogTransaksiGame.hpp"
#include "utils/NimonspoliException.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

PlayerActionService::PlayerActionService( MovementController* mc, ManagerPenjara* mp, ManagerProperti* mpr, ManagerFestival* mf, ManagerTransaksi* mt, Papan* papan, LogTransaksiGame* log, std::vector<Pemain*>* pemain, int* turnIdx) : movementController(mc), managerPenjara(mp), managerProperti(mpr), managerFestival(mf), managerTransaksi(mt), papan(papan), logger(log), daftarPemain(pemain), arahNormal(true), currentTurnIdx(turnIdx) {}

void PlayerActionService::logAksi(Pemain& pemain, const std::string& aksi, const std::string& detail) {
    if (logger) {
        logger->tambahLog(LogTransaksiEntry(0, pemain.getUsername(), aksi, detail));
    }
}

void PlayerActionService::logFestivalActivation(Pemain& pemain, PetakProperti& prop, int pengali, int durasi) {
    std::string detail = prop.getKode() + ": sewa x" + std::to_string(pengali) + ", durasi " + std::to_string(durasi) + " giliran";
    logAksi(pemain, "FESTIVAL", detail);
}

void PlayerActionService::transferMoney(Pemain* asal, Pemain* tujuan, int jumlah) {
    managerTransaksi->transferMoney(asal, tujuan, jumlah);
}

void PlayerActionService::collectFromAllPlayers(Pemain& penerima, int jumlahPerOrang) {
    managerTransaksi->collectFromAllPlayers(penerima, jumlahPerOrang);
}

void PlayerActionService::payToAllPlayers(Pemain& pembayar, int jumlahPerOrang) {
    managerTransaksi->payToAllPlayers(pembayar, jumlahPerOrang);
}

void PlayerActionService::movePlayerRelative(Pemain& p, int n) {
    // stub: MovementController::moveSteps() akan dipakai saat integrasi
    int totalPetak = papan ? papan->getTotalPetak() : 40;
    int posiBaru = ((p.getPosisi() + n) % totalPetak + totalPetak) % totalPetak;
    p.setPosisi(posiBaru);
    OutputHandler::cetakAksi(p.getUsername(), "bergerak ke petak " + std::to_string(posiBaru));
    logAksi(p, "GERAK", "pindah ke indeks " + std::to_string(posiBaru));
}

void PlayerActionService::teleportByInput(Pemain& p, const std::string& kodeOrIndeks) {
    // stub: validasi kode petak di Papan akan dilakukan saat integrasi
    OutputHandler::cetakAksi(p.getUsername(), "teleport ke " + kodeOrIndeks);
    logAksi(p, "TELEPORT", kodeOrIndeks);
}

void PlayerActionService::moveToNearestStation(Pemain& p) {
    // stub: MovementController::findNearestStation() saat integrasi
    OutputHandler::cetakAksi(p.getUsername(), "bergerak ke stasiun terdekat.");
    logAksi(p, "GERAK", "stasiun terdekat");
}

void PlayerActionService::sendToJail(Pemain& p) {
    // stub: ManagerPenjara::masukkanKePenjara() saat integrasi
    p.setStatus(StatusPemain::JAILED);
    p.resetPercobaanPenjara();
    OutputHandler::cetakAksi(p.getUsername(), "masuk penjara!");
    logAksi(p, "PENJARA", "masuk penjara");
}

void PlayerActionService::escapeFromJailByCard(Pemain& p) {
    p.setStatus(StatusPemain::ACTIVE);
    p.resetPercobaanPenjara();
    OutputHandler::cetakAksi(p.getUsername(), "bebas dari penjara (kartu).");
    logAksi(p, "PENJARA", "bebas via kartu");
}

void PlayerActionService::jailOpponent(Pemain& pemain) {
    OutputHandler::cetakPesan("Pilih pemain yang ingin dipenjara:");
    std::vector<Pemain*> aktif;
    for (Pemain* p : *daftarPemain) {
        if (p != &pemain && p->getStatus() == StatusPemain::ACTIVE) aktif.push_back(p);
    }
    if (aktif.empty()) {
        OutputHandler::cetakError("Tidak ada pemain lain yang aktif.");
        return;
    }
    for (size_t i = 0; i < aktif.size(); ++i)
        OutputHandler::cetakPesan("  " + std::to_string(i+1) + ". " + aktif[i]->getUsername());
    int pilihan = InputHandler::promptAngka("Pilih (1-" + std::to_string(aktif.size()) + "): ", 1, aktif.size());
    sendToJail(*aktif[pilihan - 1]);
    logAksi(pemain, "KARTU", "PenjaraKanCard " + aktif[pilihan-1]->getUsername());
}

void PlayerActionService::bayarSewa(Pemain& penyewa, PetakProperti& properti, int nilaiDadu) {
    (void)nilaiDadu;
}
void PlayerActionService::beliProperti(Pemain& pemain, PetakProperti& properti) {
    (void)pemain; (void)properti;
}
void PlayerActionService::gadaiProperti(Pemain& pemain, PetakProperti& properti) {
    (void)pemain; (void)properti;
}
void PlayerActionService::bangunProperti(Pemain& pemain, PetakLahan& properti) {
    (void)pemain; (void)properti;
}

void PlayerActionService::demolishOpponentProperty(Pemain& pemain) {
    std::vector<Pemain*> aktif;
    for (Pemain* p : *daftarPemain) {
        if (p != &pemain && p->getStatus() == StatusPemain::ACTIVE) aktif.push_back(p);
    }
    if (aktif.empty()) {
        OutputHandler::cetakError("Tidak ada pemain lain yang aktif.");
        return;
    }

    OutputHandler::cetakPesan("Pilih pemain yang propertinya ingin dihancurkan:");
    for (size_t i = 0; i < aktif.size(); ++i)
        OutputHandler::cetakPesan("  " + std::to_string(i+1) + ". " + aktif[i]->getUsername());
    int pilihanPemain = InputHandler::promptAngka("Pilih (1-" + std::to_string(aktif.size()) + "): ", 1, aktif.size());
    Pemain* target = aktif[pilihanPemain - 1];

    std::vector<PetakLahan*> punyaBangunan = managerProperti->getPropertiBisaDihancurkan(target);
    
    if (punyaBangunan.empty()) {
        OutputHandler::cetakError(target->getUsername() + " tidak punya properti Lahan dengan bangunan");
        return;
    }

    OutputHandler::cetakPesan("Pilih properti yang bangunannya ingin dihancurkan:");
    for (size_t i = 0; i < punyaBangunan.size(); ++i) {
        PetakLahan* s = punyaBangunan[i];
        OutputHandler::cetakPesan("  " + std::to_string(i+1) + ". " + s->getNama() + " (" + s->getKode() + ") — " + (s->punyaHotel() ? "Hotel" : std::to_string(s->getJumlahBangunan()) + " rumah"));
    }

    int pilihanProperti = InputHandler::promptAngka("Pilih (1-" + std::to_string(punyaBangunan.size()) + "): ", 1, punyaBangunan.size());
    PetakLahan* dipilih = punyaBangunan[pilihanProperti - 1];

    int jumlahSebelum = managerProperti->hancurkanSemuaBangunan(dipilih);

    OutputHandler::cetakEfekKartu("DemolitionCard", "Semua bangunan (" + std::to_string(jumlahSebelum) + (jumlahSebelum == 5 ? " — hotel" : " rumah") + ") di " + dipilih->getNama() + " milik " + target->getUsername() + " telah dihancurkan.");
    logAksi(pemain, "KARTU", "DemolitionCard " + dipilih->getKode() + " milik " + target->getUsername());
}

void PlayerActionService::pullPlayerAhead(Pemain& pemain) {
    int posiSaya = pemain.getPosisi();
    int TOTAL_PETAK = papan ? papan->getTotalPetak() : 40;
    std::vector<Pemain*> kandidat;
    for (Pemain* p : *daftarPemain) {
        if (p == &pemain) continue;
        if (p->getStatus() != StatusPemain::ACTIVE) continue; // skip JAILED
        int jarak = (p->getPosisi() - posiSaya + TOTAL_PETAK) % TOTAL_PETAK;
        if (jarak > 0) kandidat.push_back(p);
    }
    if (kandidat.empty()) {
        OutputHandler::cetakError("Tidak ada pemain aktif di depan kamu.");
        return;
    }

    // urutkan dari yang terdekat
    std::sort(kandidat.begin(), kandidat.end(), [&](Pemain* a, Pemain* b) {
        int ja = (a->getPosisi() - posiSaya + TOTAL_PETAK) % TOTAL_PETAK;
        int jb = (b->getPosisi() - posiSaya + TOTAL_PETAK) % TOTAL_PETAK;
        return ja < jb;
    });
    OutputHandler::cetakPesan("Pilih pemain yang ingin ditarik ke posisi kamu (petak " + std::to_string(posiSaya) + "):");
    for (size_t i = 0; i < kandidat.size(); ++i) {
        int jarak = (kandidat[i]->getPosisi() - posiSaya + TOTAL_PETAK) % TOTAL_PETAK;
        OutputHandler::cetakPesan("  " + std::to_string(i+1) + ". " + kandidat[i]->getUsername() + " (petak " + std::to_string(kandidat[i]->getPosisi()) + ", jarak " + std::to_string(jarak) + ")");
    }
    int pilihan = 0;
    while (pilihan < 1 || pilihan > static_cast<int>(kandidat.size())) {
        std::cout << "Pilih (1-" << kandidat.size() << "): ";
        std::cin >> pilihan;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    Pemain* target = kandidat[pilihan - 1];
    OutputHandler::cetakAksi(target->getUsername(), "ditarik ke posisi " + pemain.getUsername() + " (petak " + std::to_string(posiSaya) + ")");
    target->setPosisi(posiSaya);
    // TODO (integrasi): papan->getPetak(posiSaya)->onLanded(*target, *this)
    logAksi(pemain, "KARTU", "LassoCard : tarik " + target->getUsername() + " ke petak " + std::to_string(posiSaya));
}

void PlayerActionService::rotateAllHandCards(Pemain& pemain) {
    std::vector<Pemain*> aktif;
    for (Pemain* p : *daftarPemain) {
        if (p->getStatus() == StatusPemain::ACTIVE) aktif.push_back(p);
    }
    if (aktif.size() < 2) {
        OutputHandler::cetakError("Tidak cukup pemain untuk rotasi kartu.");
        return;
    }
    // simpan hand pemain terakhir sebagai buffer
    std::vector<KartuKemampuanSpesial*> buffer = aktif.back()->getKartuDiTangan();
    // proses rotasi: hand[i] mendapat hand[i-1], dari belakang ke depan
    for (int i = static_cast<int>(aktif.size()) - 1; i > 0; --i) {
        aktif[i]->setKartuDiTangan(aktif[i - 1]->getKartuDiTangan());
    }
    aktif[0]->setKartuDiTangan(buffer);
    OutputHandler::cetakEfekKartu("RotasiKartuCard", "Kartu tangan semua pemain telah dirotasi.");
    logAksi(pemain, "KARTU", "RotasiKartuCard — rotasi " + std::to_string(aktif.size()) + " pemain");
}

void PlayerActionService::reverseTurnOrder(Pemain& pemain) {
    arahNormal = !arahNormal;
    OutputHandler::cetakEfekKartu("ReverseCard", std::string("Urutan giliran kini ") + (arahNormal ? "normal (searah jarum jam)." : "terbalik!"));
    logAksi(pemain, "KARTU", arahNormal ? "ReverseCard (normal)" : "ReverseCard (terbalik)");
}

void PlayerActionService::beriSemuaAset (Pemain* asal, Pemain *tujuan) {
    managerTransaksi->beriSemuaAset(asal, tujuan);
}
