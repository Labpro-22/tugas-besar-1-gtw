<<<<<<< HEAD
#include "models/Managers/ManagerTransaksi.hpp"
#include "models/PlayerActionService.hpp"
#include "models/Managers/ManagerProperti.hpp"
#include "models/Managers/ManagerFestival.hpp"
#include "models/Managers/ManagerPenjara.hpp"
#include "models/Managers/ManagerLelang.hpp"
#include "models/Pemain.hpp"
#include "models/Petak/PetakProperti.hpp"
#include "models/Petak/PetakLahan.hpp"
#include "models/Petak/PetakStasiun.hpp"
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

PlayerActionService::PlayerActionService(
    MovementController* mc,
    ManagerPenjara* mp,
    ManagerProperti* mpr,
    ManagerFestival* mf,
    ManagerTransaksi* mt,
    ConfigData* cfg,
    Papan* papan,
    LogTransaksiGame* log,
    std::vector<Pemain*>* pemain,
    int* turnIdx
) : movementController(mc),
    managerPenjara(mp),
    managerProperti(mpr),
    managerFestival(mf),
    managerTransaksi(mt),
    config(cfg),
    papan(papan),
    logger(log),
    daftarPemain(pemain),
    arahNormal(true),
    currentTurnIdx(turnIdx),
    currentGlobalTurn(nullptr) {}

void PlayerActionService::logAksi(Pemain& pemain, const std::string& aksi, const std::string& detail) {
    if (logger) {
        int ronde = (currentGlobalTurn != nullptr) ? *currentGlobalTurn : 0;
        logger->tambahLog(LogTransaksiEntry(ronde, pemain.getUsername(), aksi, detail));
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
    int totalPetak = papan ? papan->getTotalPetak() : 40;
    int posisiLama = p.getPosisi();
    if (posisiLama < 1 || posisiLama > totalPetak) {
        posisiLama = 1;
    }

    // Normalisasi ke rentang [1..totalPetak] agar konsisten dengan indeks petak di papan.
    int zeroBased = ((posisiLama - 1 + n) % totalPetak + totalPetak) % totalPetak;
    int posiBaru = zeroBased + 1;

    // Sesuai spesifikasi: jika pemain berhenti tepat di GO atau melewati GO saat bergerak maju,
    // pemain menerima gaji GO.
    if (n > 0 && config) {
        int langkahMajuBersih = n % totalPetak;
        bool lewatAtauTepatGO = (langkahMajuBersih == 0) || ((posisiLama + langkahMajuBersih) > totalPetak);
        if (lewatAtauTepatGO) {
            transferMoney(nullptr, &p, config->getGajiGo());
            logAksi(p, "GO", "Menerima gaji GO M" + std::to_string(config->getGajiGo()));
        }
    }

    p.setPosisi(posiBaru);
    OutputHandler::cetakAksi(p.getUsername(), "bergerak ke petak " + std::to_string(posiBaru));
    logAksi(p, "GERAK", "pindah ke indeks " + std::to_string(posiBaru));
}

void PlayerActionService::teleportByInput(Pemain& p, const std::string& kodeOrIndeks) {
    if (!papan) return;
    Petak* petak = nullptr;
    try {
        int idx = std::stoi(kodeOrIndeks);
        petak = papan->getPetak(idx);
    } catch (...) {
        petak = papan->getPetak(kodeOrIndeks);
    }

    if (petak) {
        p.setPosisi(petak->getIndeks());
        OutputHandler::cetakAksi(p.getUsername(), "teleport ke " + petak->getNama());
        logAksi(p, "TELEPORT", petak->getNama());
        petak->onLanded(p, *this);
    } else {
        OutputHandler::cetakError("Petak tidak ditemukan.");
    }
}

void PlayerActionService::moveToNearestStation(Pemain& p) {
    if (!papan) return;
    int current = p.getPosisi();
    int nearest = -1;
    int minJarak = 999;
    int totalPetak = papan->getTotalPetak();
    for (int i = 1; i <= totalPetak; ++i) {
        Petak* petak = papan->getPetak(i);
        if (dynamic_cast<PetakStasiun*>(petak)) {
            int jarak = (i - current + totalPetak) % totalPetak;
            if (jarak > 0 && jarak < minJarak) {
                minJarak = jarak;
                nearest = i;
            }
        }
    }
    if (nearest != -1) {
        p.setPosisi(nearest);
        OutputHandler::cetakAksi(p.getUsername(), "berangkat ke stasiun terdekat.");
        logAksi(p, "GERAK", "stasiun terdekat");
        papan->getPetak(nearest)->onLanded(p, *this);
    }
}

void PlayerActionService::sendToJail(Pemain& p) {
    p.setStatus(StatusPemain::JAILED);
    p.resetPercobaanPenjara();
    if (papan) {
        for (int i=1; i<=papan->getTotalPetak(); ++i) {
            Petak* petak = papan->getPetak(i);
            if (petak->getKode() == "PEN") {
                p.setPosisi(i);
                break;
            }
        }
    }
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
    Pemain* pemilik = properti.getPemilik();
    if (!pemilik || pemilik == &penyewa || properti.getStatus() == PetakProperti::StatusProperti::MORTGAGED) {
        return; 
    }
    if (pemilik->getStatus() == StatusPemain::JAILED) {
        OutputHandler::cetakPesan("Pemilik sedang di penjara. Bebas sewa!");
        return;
    }
    
    int sewaDasar = properti.hitungSewa(nilaiDadu, *managerProperti);
    int pengaliFestival = 1;
    if (managerFestival && managerFestival->isAktif(&properti)) {
        pengaliFestival = managerFestival->getPengali(&properti);
    }
    int totalSewa = sewaDasar * pengaliFestival;
    
    OutputHandler::cetakPesan(penyewa.getUsername() + " harus membayar sewa M" + std::to_string(totalSewa) + " ke " + pemilik->getUsername());
    managerTransaksi->transferMoney(&penyewa, pemilik, totalSewa);
    logAksi(penyewa, "BAYAR_SEWA", properti.getKode() + " M" + std::to_string(totalSewa));
}

void PlayerActionService::beliProperti(Pemain& pemain, PetakProperti& properti) {
    if (properti.getStatus() != PetakProperti::StatusProperti::BANK) {
        return;
    }
    
    int hargaBeli = properti.getConfigProperti()->getHargaLahan();
    OutputHandler::cetakPesan(properti.getNama() + " (" + properti.getKode() + ") belum dimiliki.");
    OutputHandler::cetakPesan("Harga: M" + std::to_string(hargaBeli));
    
    std::string respon = InputHandler::promptString("Apakah Anda ingin membeli properti ini? (Y/N): ");
    if (respon == "Y" || respon == "y") {
        if (pemain.getSaldo() >= hargaBeli) {
            managerTransaksi->transferMoney(&pemain, nullptr, hargaBeli);
            properti.setPemilik(&pemain);
            pemain.tambahAset(&properti);
            OutputHandler::cetakAksi(pemain.getUsername(), "membeli " + properti.getNama() + " seharga M" + std::to_string(hargaBeli));
            logAksi(pemain, "BELI", properti.getKode() + " (-" + std::to_string(hargaBeli) + ")");
            return;
        } else {
            OutputHandler::cetakError("Uang tidak mencukupi untuk membeli properti ini.");
        }
    }
    
    OutputHandler::cetakPesan("Properti akan dilelang!");
    ManagerLelang lelang(managerTransaksi);
    lelang.mulaiLelang(&properti, *daftarPemain, &pemain);
}

void PlayerActionService::gadaiProperti(Pemain& pemain) {
    std::vector<PetakProperti*> bisaDigadai;
    for (PetakProperti* prop : pemain.getAsetPemain()) {
        if (prop->getStatus() == PetakProperti::StatusProperti::OWNED) {
            bisaDigadai.push_back(prop);
        }
    }
    if (bisaDigadai.empty()) {
        OutputHandler::cetakError("Tidak ada properti yang dapat digadaikan saat ini.");
        return;
    }
    OutputHandler::cetakPesan("=== Properti yang Dapat Digadaikan ===");
    for (size_t i = 0; i < bisaDigadai.size(); ++i) {
        std::string warnaOrType = "";
        PetakLahan* street = dynamic_cast<PetakLahan*>(bisaDigadai[i]);
        if (street) {
            warnaOrType = "[" + street->getWarnaString() + "] ";
        } else if (dynamic_cast<PetakStasiun*>(bisaDigadai[i])) {
            warnaOrType = "[STASIUN] ";
        } else if (dynamic_cast<PetakUtilitas*>(bisaDigadai[i])) {
            warnaOrType = "[UTILITAS] ";
        }
        OutputHandler::cetakPesan(std::to_string(i+1) + ". " + bisaDigadai[i]->getNama() + " (" + bisaDigadai[i]->getKode() + ") " + warnaOrType + "Nilai Gadai: M" + std::to_string(bisaDigadai[i]->getNilaiGadai()));
    }
    int pilihan = InputHandler::promptAngka("Pilih nomor properti (0 untuk batal): ", 0, bisaDigadai.size());
    if (pilihan == 0) return;
    
    PetakProperti* prop = bisaDigadai[pilihan - 1];
    
    PetakLahan* street = dynamic_cast<PetakLahan*>(prop);
    if (street) {
        std::vector<PetakLahan*> group = managerProperti->getPropertiByGrup(street->getWarnaString());
        bool hasBuildings = false;
        for (PetakLahan* peer : group) {
            if (peer->getJumlahBangunan() > 0) {
                hasBuildings = true;
                break;
            }
        }
        
        if (hasBuildings) {
            OutputHandler::cetakError(prop->getNama() + " tidak dapat digadaikan!");
            OutputHandler::cetakPesan("Masih terdapat bangunan di color group [" + street->getWarnaString() + "].");
            OutputHandler::cetakPesan("Bangunan harus dijual terlebih dahulu.");
            OutputHandler::cetakPesan("Daftar bangunan di color group [" + street->getWarnaString() + "]:");
            int totalUangBangunan = 0;
            int displayIdx = 1;
            for (size_t k = 0; k < group.size(); ++k) {
                PetakLahan* peer = group[k];
                if (peer->getJumlahBangunan() > 0) {
                    int n_bangun = peer->getJumlahBangunan();
                    int uang_kembali = n_bangun * (peer->getHargaBangun() / 2);
                    totalUangBangunan += uang_kembali;
                    std::string target = peer->punyaHotel() ? "1 hotel" : std::to_string(n_bangun) + " rumah";
                    OutputHandler::cetakPesan(std::to_string(displayIdx) + ". " + peer->getNama() + " (" + peer->getKode() + ") - " + target + " -> Nilai jual bangunan: M" + std::to_string(uang_kembali));
                    displayIdx++;
                }
            }
            
            std::string confirm = InputHandler::promptString("Jual semua bangunan color group [" + street->getWarnaString() + "]? (y/n): ");
            if (confirm == "y" || confirm == "Y") {
                for (PetakLahan* peer : group) {
                    if (peer->getJumlahBangunan() > 0) {
                        int n_bangun = peer->getJumlahBangunan();
                        int uang_kembali = n_bangun * (peer->getHargaBangun() / 2);
                        while (peer->getJumlahBangunan() > 0) {
                            peer->turunkanBangunan();
                        }
                        OutputHandler::cetakPesan("Bangunan " + peer->getNama() + " terjual. Kamu menerima M" + std::to_string(uang_kembali) + ".");
                    }
                }
                managerTransaksi->transferMoney(nullptr, &pemain, totalUangBangunan);
                OutputHandler::cetakPesan("Uang kamu sekarang: M" + std::to_string(pemain.getSaldo()));
                logAksi(pemain, "JUAL_BANGUNAN", street->getWarnaString() + " (+" + std::to_string(totalUangBangunan) + ")");
                
                std::string confLanjut = InputHandler::promptString("Lanjut menggadaikan " + prop->getNama() + "? (y/n): ");
                if (confLanjut != "y" && confLanjut != "Y") {
                    return;
                }
            } else {
                return;
            }
        }
    }

    prop->gadai();
    int nilaiGadai = prop->getNilaiGadai();
    managerTransaksi->transferMoney(nullptr, &pemain, nilaiGadai);
    OutputHandler::cetakPesan(prop->getNama() + " berhasil digadaikan.");
    OutputHandler::cetakPesan("Kamu menerima M" + std::to_string(nilaiGadai) + " dari Bank.");
    OutputHandler::cetakPesan("Uang kamu sekarang: M" + std::to_string(pemain.getSaldo()));
    OutputHandler::cetakPesan("Catatan: Sewa tidak dapat dipungut dari properti yang digadaikan.");
    logAksi(pemain, "GADAI", prop->getKode() + " (+" + std::to_string(nilaiGadai) + ")");
}

void PlayerActionService::tebusProperti(Pemain& pemain) {
    std::vector<PetakProperti*> bisaDitebus;
    for (PetakProperti* prop : pemain.getAsetPemain()) {
        if (prop->getStatus() == PetakProperti::StatusProperti::MORTGAGED) {
            bisaDitebus.push_back(prop);
        }
    }
    if (bisaDitebus.empty()) {
        OutputHandler::cetakError("Tidak ada properti yang bisa ditebus.");
        return;
    }
    OutputHandler::cetakPesan("Pilih properti yang ingin ditebus:");
    for (size_t i = 0; i < bisaDitebus.size(); ++i) {
        int biaya = bisaDitebus[i]->getConfigProperti()->getHargaLahan(); // Tebus harga beli penuh
        OutputHandler::cetakPesan("  " + std::to_string(i+1) + ". " + bisaDitebus[i]->getNama() + " (" + bisaDitebus[i]->getKode() + ") — Biaya Tebus: M" + std::to_string(biaya));
    }
    int pilihan = InputHandler::promptAngka("Pilih (1-" + std::to_string(bisaDitebus.size()) + ", 0 batal): ", 0, bisaDitebus.size());
    if (pilihan == 0) return;
    
    PetakProperti* prop = bisaDitebus[pilihan - 1];
    int biaya = prop->getConfigProperti()->getHargaLahan(); 
    if (pemain.getSaldo() < biaya) {
        OutputHandler::cetakError("Uang tidak cukup.");
        return;
    }
    managerTransaksi->transferMoney(&pemain, nullptr, biaya);
    prop->batalGadai();
    OutputHandler::cetakPesan(pemain.getUsername() + " menebus " + prop->getNama() + " dengan membayar M" + std::to_string(biaya));
    logAksi(pemain, "TEBUS", prop->getKode() + " (-" + std::to_string(biaya) + ")");
}

void PlayerActionService::bangunProperti(Pemain& pemain) {
    std::vector<PetakLahan*> bisaDibangun;
    for (PetakProperti* prop : pemain.getAsetPemain()) {
        PetakLahan* lahan = dynamic_cast<PetakLahan*>(prop);
        if (lahan && managerProperti->isMonopoly(&pemain, lahan->getWarnaString()) && managerProperti->isBisaBangun(lahan)) {
            bisaDibangun.push_back(lahan);
        }
    }
    if (bisaDibangun.empty()) {
        OutputHandler::cetakError("Tidak ada lahan yang dapat dibangun saat ini. Syarat: Monopoli warna, properti tidak digadai, maksimal 1 Hotel.");
        return;
    }
    OutputHandler::cetakPesan("Pilih lahan untuk dibangun:");
    for (size_t i = 0; i < bisaDibangun.size(); ++i) {
        int harga = bisaDibangun[i]->getHargaBangun();
        std::string target = (bisaDibangun[i]->getJumlahBangunan() == 4) ? "Hotel" : "Rumah ke-" + std::to_string(bisaDibangun[i]->getJumlahBangunan() + 1);
        OutputHandler::cetakPesan("  " + std::to_string(i+1) + ". " + bisaDibangun[i]->getNama() + " (" + bisaDibangun[i]->getKode() + ") — Bangun " + target + ": M" + std::to_string(harga));
    }
    int pilihan = InputHandler::promptAngka("Pilih (1-" + std::to_string(bisaDibangun.size()) + ", 0 batal): ", 0, bisaDibangun.size());
    if (pilihan == 0) return;
    
    PetakLahan* lahan = bisaDibangun[pilihan - 1];
    int biaya = lahan->getHargaBangun();
    if (pemain.getSaldo() < biaya) {
        OutputHandler::cetakError("Uang tidak cukup.");
        return;
    }
    managerTransaksi->transferMoney(&pemain, nullptr, biaya);
    lahan->bangun(*managerProperti);
    OutputHandler::cetakAksi(pemain.getUsername(), "berhasil membangun di " + lahan->getNama() + " dengan biaya M" + std::to_string(biaya));
    logAksi(pemain, "BANGUN", lahan->getKode() + " (-" + std::to_string(biaya) + ")");
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
    if (papan) {
        Petak* petakTujuan = papan->getPetak(posiSaya);
        if (petakTujuan) {
            petakTujuan->onLanded(*target, *this);
        }
    }
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
=======
#include "models/Managers/ManagerTransaksi.hpp"
#include "models/PlayerActionService.hpp"
#include "models/Managers/ManagerProperti.hpp"
#include "models/Managers/ManagerFestival.hpp"
#include "models/Managers/ManagerPenjara.hpp"
#include "models/Managers/ManagerLelang.hpp"
#include "models/Pemain.hpp"
#include "models/Petak/PetakProperti.hpp"
#include "models/Petak/PetakLahan.hpp"
#include "models/Petak/PetakStasiun.hpp"
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

PlayerActionService::PlayerActionService(
    MovementController* mc,
    ManagerPenjara* mp,
    ManagerProperti* mpr,
    ManagerFestival* mf,
    ManagerTransaksi* mt,
    ConfigData* cfg,
    Papan* papan,
    LogTransaksiGame* log,
    std::vector<Pemain*>* pemain,
    int* turnIdx
) : movementController(mc),
    managerPenjara(mp),
    managerProperti(mpr),
    managerFestival(mf),
    managerTransaksi(mt),
    config(cfg),
    papan(papan),
    logger(log),
    daftarPemain(pemain),
    arahNormal(true),
    currentTurnIdx(turnIdx) {}

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
    if (!papan) return;
    Petak* petak = nullptr;
    try {
        int idx = std::stoi(kodeOrIndeks);
        petak = papan->getPetak(idx);
    } catch (...) {
        petak = papan->getPetak(kodeOrIndeks);
    }

    if (petak) {
        p.setPosisi(petak->getIndeks());
        OutputHandler::cetakAksi(p.getUsername(), "teleport ke " + petak->getNama());
        logAksi(p, "TELEPORT", petak->getNama());
        petak->onLanded(p, *this);
    } else {
        OutputHandler::cetakError("Petak tidak ditemukan.");
    }
}

void PlayerActionService::moveToNearestStation(Pemain& p) {
    if (!papan) return;
    int current = p.getPosisi();
    int nearest = -1;
    int minJarak = 999;
    int totalPetak = papan->getTotalPetak();
    for (int i = 1; i <= totalPetak; ++i) {
        Petak* petak = papan->getPetak(i);
        if (dynamic_cast<PetakStasiun*>(petak)) {
            int jarak = (i - current + totalPetak) % totalPetak;
            if (jarak > 0 && jarak < minJarak) {
                minJarak = jarak;
                nearest = i;
            }
        }
    }
    if (nearest != -1) {
        p.setPosisi(nearest);
        OutputHandler::cetakAksi(p.getUsername(), "berangkat ke stasiun terdekat.");
        logAksi(p, "GERAK", "stasiun terdekat");
        papan->getPetak(nearest)->onLanded(p, *this);
    }
}

void PlayerActionService::sendToJail(Pemain& p) {
    p.setStatus(StatusPemain::JAILED);
    p.resetPercobaanPenjara();
    if (papan) {
        for (int i=1; i<=papan->getTotalPetak(); ++i) {
            Petak* petak = papan->getPetak(i);
            if (petak->getKode() == "PEN") {
                p.setPosisi(i);
                break;
            }
        }
    }
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
    Pemain* pemilik = properti.getPemilik();
    if (!pemilik || pemilik == &penyewa || properti.getStatus() == PetakProperti::StatusProperti::MORTGAGED) {
        return; 
    }
    if (pemilik->getStatus() == StatusPemain::JAILED) {
        OutputHandler::cetakPesan("Pemilik sedang di penjara. Bebas sewa!");
        return;
    }
    
    int sewaDasar = properti.hitungSewa(nilaiDadu, *managerProperti);
    int pengaliFestival = 1;
    if (managerFestival && managerFestival->isAktif(&properti)) {
        pengaliFestival = managerFestival->getPengali(&properti);
    }
    int totalSewa = sewaDasar * pengaliFestival;
    
    OutputHandler::cetakPesan(penyewa.getUsername() + " harus membayar sewa M" + std::to_string(totalSewa) + " ke " + pemilik->getUsername());
    managerTransaksi->transferMoney(&penyewa, pemilik, totalSewa);
    logAksi(penyewa, "BAYAR_SEWA", properti.getKode() + " M" + std::to_string(totalSewa));
}

void PlayerActionService::beliProperti(Pemain& pemain, PetakProperti& properti) {
    if (properti.getStatus() != PetakProperti::StatusProperti::BANK) {
        return;
    }
    
    int hargaBeli = properti.getConfigProperti()->getHargaLahan();
    OutputHandler::cetakPesan(properti.getNama() + " (" + properti.getKode() + ") belum dimiliki.");
    OutputHandler::cetakPesan("Harga: M" + std::to_string(hargaBeli));
    
    std::string respon = InputHandler::promptString("Apakah Anda ingin membeli properti ini? (Y/N): ");
    if (respon == "Y" || respon == "y") {
        if (pemain.getSaldo() >= hargaBeli) {
            managerTransaksi->transferMoney(&pemain, nullptr, hargaBeli);
            properti.setPemilik(&pemain);
            pemain.tambahAset(&properti);
            OutputHandler::cetakAksi(pemain.getUsername(), "membeli " + properti.getNama() + " seharga M" + std::to_string(hargaBeli));
            logAksi(pemain, "BELI", properti.getKode() + " (-" + std::to_string(hargaBeli) + ")");
            return;
        } else {
            OutputHandler::cetakError("Uang tidak mencukupi untuk membeli properti ini.");
        }
    }
    
    OutputHandler::cetakPesan("Properti akan dilelang!");
    ManagerLelang lelang(managerTransaksi);
    lelang.mulaiLelang(&properti, *daftarPemain, &pemain);
}

void PlayerActionService::gadaiProperti(Pemain& pemain) {
    std::vector<PetakProperti*> bisaDigadai;
    for (PetakProperti* prop : pemain.getAsetPemain()) {
        if (prop->getStatus() == PetakProperti::StatusProperti::OWNED) {
            PetakLahan* street = dynamic_cast<PetakLahan*>(prop);
            if (!street || street->getJumlahBangunan() == 0) {
                bisaDigadai.push_back(prop);
            }
        }
    }
    if (bisaDigadai.empty()) {
        OutputHandler::cetakError("Tidak ada properti yang bisa digadai.");
        return;
    }
    OutputHandler::cetakPesan("Pilih properti yang ingin digadai:");
    for (size_t i = 0; i < bisaDigadai.size(); ++i) {
        OutputHandler::cetakPesan("  " + std::to_string(i+1) + ". " + bisaDigadai[i]->getNama() + " (" + bisaDigadai[i]->getKode() + ") — Nilai: M" + std::to_string(bisaDigadai[i]->getNilaiGadai()));
    }
    int pilihan = InputHandler::promptAngka("Pilih (1-" + std::to_string(bisaDigadai.size()) + ", 0 batal): ", 0, bisaDigadai.size());
    if (pilihan == 0) return;
    
    PetakProperti* prop = bisaDigadai[pilihan - 1];
    prop->gadai();
    int nilaiGadai = prop->getNilaiGadai();
    managerTransaksi->transferMoney(nullptr, &pemain, nilaiGadai);
    OutputHandler::cetakPesan(pemain.getUsername() + " menggadaikan " + prop->getNama() + " dan mendapat M" + std::to_string(nilaiGadai));
    logAksi(pemain, "GADAI", prop->getKode() + " (+" + std::to_string(nilaiGadai) + ")");
}

void PlayerActionService::tebusProperti(Pemain& pemain) {
    std::vector<PetakProperti*> bisaDitebus;
    for (PetakProperti* prop : pemain.getAsetPemain()) {
        if (prop->getStatus() == PetakProperti::StatusProperti::MORTGAGED) {
            bisaDitebus.push_back(prop);
        }
    }
    if (bisaDitebus.empty()) {
        OutputHandler::cetakError("Tidak ada properti yang bisa ditebus.");
        return;
    }
    OutputHandler::cetakPesan("Pilih properti yang ingin ditebus:");
    for (size_t i = 0; i < bisaDitebus.size(); ++i) {
        int biaya = bisaDitebus[i]->getConfigProperti()->getHargaLahan(); // Tebus harga beli penuh
        OutputHandler::cetakPesan("  " + std::to_string(i+1) + ". " + bisaDitebus[i]->getNama() + " (" + bisaDitebus[i]->getKode() + ") — Biaya Tebus: M" + std::to_string(biaya));
    }
    int pilihan = InputHandler::promptAngka("Pilih (1-" + std::to_string(bisaDitebus.size()) + ", 0 batal): ", 0, bisaDitebus.size());
    if (pilihan == 0) return;
    
    PetakProperti* prop = bisaDitebus[pilihan - 1];
    int biaya = prop->getConfigProperti()->getHargaLahan(); 
    if (pemain.getSaldo() < biaya) {
        OutputHandler::cetakError("Uang tidak cukup.");
        return;
    }
    managerTransaksi->transferMoney(&pemain, nullptr, biaya);
    prop->batalGadai();
    OutputHandler::cetakPesan(pemain.getUsername() + " menebus " + prop->getNama() + " dengan membayar M" + std::to_string(biaya));
    logAksi(pemain, "TEBUS", prop->getKode() + " (-" + std::to_string(biaya) + ")");
}

void PlayerActionService::bangunProperti(Pemain& pemain) {
    std::vector<PetakLahan*> bisaDibangun;
    for (PetakProperti* prop : pemain.getAsetPemain()) {
        PetakLahan* lahan = dynamic_cast<PetakLahan*>(prop);
        if (lahan && managerProperti->isMonopoly(&pemain, lahan->getWarnaString()) && managerProperti->isBisaBangun(lahan)) {
            bisaDibangun.push_back(lahan);
        }
    }
    if (bisaDibangun.empty()) {
        OutputHandler::cetakError("Tidak ada lahan yang dapat dibangun saat ini. Syarat: Monopoli warna, properti tidak digadai, maksimal 1 Hotel.");
        return;
    }
    OutputHandler::cetakPesan("Pilih lahan untuk dibangun:");
    for (size_t i = 0; i < bisaDibangun.size(); ++i) {
        int harga = bisaDibangun[i]->getHargaBangun();
        std::string target = (bisaDibangun[i]->getJumlahBangunan() == 4) ? "Hotel" : "Rumah ke-" + std::to_string(bisaDibangun[i]->getJumlahBangunan() + 1);
        OutputHandler::cetakPesan("  " + std::to_string(i+1) + ". " + bisaDibangun[i]->getNama() + " (" + bisaDibangun[i]->getKode() + ") — Bangun " + target + ": M" + std::to_string(harga));
    }
    int pilihan = InputHandler::promptAngka("Pilih (1-" + std::to_string(bisaDibangun.size()) + ", 0 batal): ", 0, bisaDibangun.size());
    if (pilihan == 0) return;
    
    PetakLahan* lahan = bisaDibangun[pilihan - 1];
    int biaya = lahan->getHargaBangun();
    if (pemain.getSaldo() < biaya) {
        OutputHandler::cetakError("Uang tidak cukup.");
        return;
    }
    managerTransaksi->transferMoney(&pemain, nullptr, biaya);
    lahan->bangun(*managerProperti);
    OutputHandler::cetakAksi(pemain.getUsername(), "berhasil membangun di " + lahan->getNama() + " dengan biaya M" + std::to_string(biaya));
    logAksi(pemain, "BANGUN", lahan->getKode() + " (-" + std::to_string(biaya) + ")");
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
>>>>>>> 4c863580e8432dbd781567a56c63fe50b90b3f39
