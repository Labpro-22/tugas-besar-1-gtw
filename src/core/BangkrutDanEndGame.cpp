<<<<<<< HEAD
#include "views/OutputHandler.hpp"
#include "views/InputHandler.hpp"
#include "core/BangkrutDanEndGame.hpp"
#include "models/ConfigData.hpp"
const ConfigData* Likuidasi::config = nullptr;
#include "../../include/core/BangkrutDanEndGame.hpp"

// Kebangkrutan
void Kebangkrutan::declareBangkrut (Pemain *p) {
    p->setStatus(StatusPemain::BANKRUPT);
}

void Kebangkrutan::tampilkanBangkrut (Pemain *p) {
    OutputHandler::cetakPesan(p->getUsername() + " telah bangkrut dan dikeluarkan dari permainan");
}

// Likuidasi
bool Likuidasi::bisaBayarDenganLikuidasi (Pemain *p, int harga) {
    return totalLikuidasiSeluruhAsetJual(p) >= harga;
}

bool Likuidasi::bisaBayarLangsung (Pemain* p, int harga) {
    return p->getSaldo() >= harga;
}


int Likuidasi::totalLikuidasiSeluruhAsetGadai (Pemain *p) {
    int res = p->getSaldo();

    for (auto aset : p->getAsetPemain()) {
        if (aset->getStatus() != PetakProperti::StatusProperti::OWNED) {
            // Properti yang sudah MORTGAGED tidak bisa digadai lagi.
            continue;
        }
        res += aset->getNilaiGadai();

        auto street = dynamic_cast<PetakLahan*>(aset);
        if(street){
            int level = street->getJumlahBangunan();
            res += level * (street->getHargaBangun() / 2);
        }
    }

    return res;
}

int Likuidasi::totalLikuidasiSeluruhAsetJual (Pemain *p) {
    int res = p->getSaldo();

    for (auto aset : p->getAsetPemain()) {
        if (aset->getStatus() != PetakProperti::StatusProperti::OWNED) {
            // Sesuai spesifikasi, properti MORTGAGED tidak bisa langsung dijual (harus tebus dulu).
            continue;
        }
        res += aset->getConfigProperti()->getHargaLahan();

        auto street = dynamic_cast<PetakLahan*>(aset);
        if(street){
            int level = street->getJumlahBangunan();
            res += level * (street->getHargaBangun() / 2);
        }
    }

    return res;
}

void Likuidasi::tampilkanPanelLikuidasi (Pemain *p, int harga) {
    if (p == nullptr) return;

    while (!bisaBayarLangsung(p, harga)) {
        std::vector<PetakProperti*> daftarJual;
        std::vector<PetakProperti*> daftarGadai;
        std::vector<PetakProperti*> daftarTebus;

        for (PetakProperti* aset : p->getAsetPemain()) {
            if (aset == nullptr) continue;
            if (aset->getStatus() == PetakProperti::StatusProperti::OWNED) {
                daftarJual.push_back(aset);
                daftarGadai.push_back(aset);
            } else if (aset->getStatus() == PetakProperti::StatusProperti::MORTGAGED) {
                daftarTebus.push_back(aset);
            }
        }

        OutputHandler::cetakPesan("=== Panel Likuidasi ===");
        OutputHandler::cetakPesan("Uang kamu saat ini: M" + std::to_string(p->getSaldo()));
        OutputHandler::cetakPesan("Kewajiban: M" + std::to_string(harga));

        int idx = 1;
        std::vector<std::pair<int, PetakProperti*>> mapJual;
        std::vector<std::pair<int, PetakProperti*>> mapGadai;
        std::vector<std::pair<int, PetakProperti*>> mapTebus;

        if (!daftarJual.empty()) {
            OutputHandler::cetakPesan("[Jual ke Bank]");
            for (PetakProperti* aset : daftarJual) {
                int hargaJual = aset->getConfigProperti()->getHargaLahan();
                auto street = dynamic_cast<PetakLahan*>(aset);
                if (street) {
                    hargaJual += street->getJumlahBangunan() * (street->getHargaBangun() / 2);
                }
                OutputHandler::cetakPesan(std::to_string(idx) + ". " + aset->getNama() + " (" + aset->getKode() + ") -> Harga Jual: M" + std::to_string(hargaJual));
                mapJual.push_back({idx, aset});
                idx++;
            }
        }

        if (!daftarGadai.empty()) {
            OutputHandler::cetakPesan("[Gadaikan]");
            for (PetakProperti* aset : daftarGadai) {
                OutputHandler::cetakPesan(std::to_string(idx) + ". " + aset->getNama() + " (" + aset->getKode() + ") -> Nilai Gadai: M" + std::to_string(aset->getNilaiGadai()));
                mapGadai.push_back({idx, aset});
                idx++;
            }
        }

        if (!daftarTebus.empty()) {
            OutputHandler::cetakPesan("[Tebus terlebih dahulu]");
            for (PetakProperti* aset : daftarTebus) {
                int hargaTebus = aset->getConfigProperti()->getHargaLahan();
                OutputHandler::cetakPesan(std::to_string(idx) + ". " + aset->getNama() + " (" + aset->getKode() + ") [M] -> Harga Tebus: M" + std::to_string(hargaTebus));
                mapTebus.push_back({idx, aset});
                idx++;
            }
        }

        if (idx == 1) {
            OutputHandler::cetakError("Tidak ada aksi likuidasi yang bisa dilakukan lagi.");
            return;
        }

        int pilihan = InputHandler::promptAngka("Pilih aksi (0 jika sudah cukup): ", 0, idx - 1);
        if (pilihan == 0) {
            break;
        }

        bool aksiTerproses = false;
        for (const auto& item : mapJual) {
            if (item.first == pilihan) {
                PetakProperti* aset = item.second;
                int hargaJual = aset->getConfigProperti()->getHargaLahan();

                auto street = dynamic_cast<PetakLahan*>(aset);
                if (street) {
                    int jumlahBangunan = street->getJumlahBangunan();
                    hargaJual += jumlahBangunan * (street->getHargaBangun() / 2);
                    while (street->getJumlahBangunan() > 0) {
                        street->turunkanBangunan();
                    }
                }

                p->hapusAset(aset);
                aset->setPemilik(nullptr);
                aset->setStatus(PetakProperti::StatusProperti::BANK);
                *p += hargaJual;

                OutputHandler::cetakPesan(aset->getNama() + " terjual ke Bank. Kamu menerima M" + std::to_string(hargaJual) + ".");
                aksiTerproses = true;
                break;
            }
        }
        if (aksiTerproses) continue;

        for (const auto& item : mapGadai) {
            if (item.first == pilihan) {
                PetakProperti* aset = item.second;
                auto street = dynamic_cast<PetakLahan*>(aset);

                if (street) {
                    std::vector<PetakLahan*> group;
                    for (PetakProperti* peerAset : p->getAsetPemain()) {
                        auto peerStreet = dynamic_cast<PetakLahan*>(peerAset);
                        if (peerStreet && peerStreet->getWarnaString() == street->getWarnaString()) {
                            group.push_back(peerStreet);
                        }
                    }

                    bool hasBuildings = false;
                    for (PetakLahan* peer : group) {
                        if (peer->getJumlahBangunan() > 0) {
                            hasBuildings = true;
                            break;
                        }
                    }

                    if (hasBuildings) {
                        OutputHandler::cetakError(aset->getNama() + " tidak dapat digadaikan!");
                        OutputHandler::cetakPesan("Masih terdapat bangunan di color group [" + street->getWarnaString() + "].");
                        OutputHandler::cetakPesan("Bangunan harus dijual terlebih dahulu.");

                        int totalUangBangunan = 0;
                        for (PetakLahan* peer : group) {
                            int nBangun = peer->getJumlahBangunan();
                            if (nBangun == 0) continue;
                            totalUangBangunan += nBangun * (peer->getHargaBangun() / 2);
                        }

                        std::string confirm = InputHandler::promptString("Jual semua bangunan color group [" + street->getWarnaString() + "]? (y/n): ");
                        if (confirm != "y" && confirm != "Y") {
                            aksiTerproses = true;
                            break;
                        }

                        for (PetakLahan* peer : group) {
                            if (peer->getJumlahBangunan() > 0) {
                                int nBangun = peer->getJumlahBangunan();
                                int uangKembali = nBangun * (peer->getHargaBangun() / 2);
                                while (peer->getJumlahBangunan() > 0) {
                                    peer->turunkanBangunan();
                                }
                                OutputHandler::cetakPesan("Bangunan " + peer->getNama() + " terjual. Kamu menerima M" + std::to_string(uangKembali) + ".");
                            }
                        }
                        *p += totalUangBangunan;
                        OutputHandler::cetakPesan("Uang kamu sekarang: M" + std::to_string(p->getSaldo()));
                    }
                }

                aset->gadai();
                *p += aset->getNilaiGadai();
                OutputHandler::cetakPesan(aset->getNama() + " berhasil digadaikan. Kamu menerima M" + std::to_string(aset->getNilaiGadai()) + ".");
                aksiTerproses = true;
                break;
            }
        }
        if (aksiTerproses) continue;

        for (const auto& item : mapTebus) {
            if (item.first == pilihan) {
                PetakProperti* aset = item.second;
                int hargaTebus = aset->getConfigProperti()->getHargaLahan();
                if (p->getSaldo() < hargaTebus) {
                    OutputHandler::cetakError("Uang kamu tidak cukup untuk menebus " + aset->getNama() + ".");
                    aksiTerproses = true;
                    break;
                }

                *p -= hargaTebus;
                aset->batalGadai();
                OutputHandler::cetakPesan(aset->getNama() + " berhasil ditebus.");
                aksiTerproses = true;
                break;
            }
        }
    }
}

// SelesaiGame
Pemain* SelesaiGame::getPemenang (vector<Pemain*> listPemain) {
    Pemain* pemenang = nullptr;

    for (auto player : listPemain) {
        if (player->getStatus() != StatusPemain::BANKRUPT) {
            if (pemenang == nullptr || *player > *pemenang) {
                pemenang = player;
            }
        }
    }

    return pemenang;
    
}

bool SelesaiGame::sisaSatuOrang (vector<Pemain*> listPemain) {
    int count = 0;

    for (auto player : listPemain) {
        // Pemain yang masih JAILED belum bangkrut dan tetap dihitung sebagai pemain aktif game.
        if (player->getStatus() != StatusPemain::BANKRUPT) {
            count++;
        }
    }

    return count == 1;
}

void SelesaiGame::tampilPanelSelesaiKarenaMaxTurn (int maxTurn) {
    OutputHandler::cetakPesan("Permainan Selesai! Sudah mencapai " + std::to_string(maxTurn) + " giliran!");
}

void SelesaiGame::tampilPanelSelesaiKarenaBangkrut () {
    OutputHandler::cetakPesan("Permainan Selesai! Semua pemain kecuali satu pemain bangkrut");
}

void SelesaiGame::tampilkanPemenang (Pemain *p) {
    OutputHandler::cetakPesan("Pemenang permainan ini adalah " + p->getUsername());
}
=======
#include "views/OutputHandler.hpp"
#include "core/BangkrutDanEndGame.hpp"
#include "models/ConfigData.hpp"
const ConfigData* Likuidasi::config = nullptr;
#include "../../include/core/BangkrutDanEndGame.hpp"

// Kebangkrutan
void Kebangkrutan::declareBangkrut (Pemain *p) {
    p->setStatus(StatusPemain::BANKRUPT);
}

void Kebangkrutan::tampilkanBangkrut (Pemain *p) {
    OutputHandler::cetakPesan(p->getUsername() + " telah bangkrut dan dikeluarkan dari permainan");
}

// Likuidasi
bool Likuidasi::bisaBayarDenganLikuidasi (Pemain *p, int harga) {
    return totalLikuidasiSeluruhAsetJual(p) >= harga;
}

bool Likuidasi::bisaBayarLangsung (Pemain* p, int harga) {
    return p->getSaldo() >= harga;
}


int Likuidasi::totalLikuidasiSeluruhAsetGadai (Pemain *p) {
    int res = p->getSaldo();

    for (auto aset : p->getAsetPemain()) {
        res += aset->getNilaiGadai();

        auto street = dynamic_cast<PetakLahan*>(aset);
        if(street){
            int level = street->getJumlahBangunan();
            res += level * (street->getHargaBangun() / 2);
        }
    }

    return res;
}

int Likuidasi::totalLikuidasiSeluruhAsetJual (Pemain *p) {
    int res = p->getSaldo();

    for (auto aset : p->getAsetPemain()) {
        res += aset->getConfigProperti()->getHargaLahan();

        auto street = dynamic_cast<PetakLahan*>(aset);
        if(street){
            int level = street->getJumlahBangunan();
            res += level * (street->getHargaBangun() / 2);
        }
    }

    return res;
}

void Likuidasi::tampilkanPanelLikuidasi (Pemain *p, int harga) {
    if (config) {
        OutputHandler::cetakPropertiPemain(p, *config);
        OutputHandler::cetakPesan("Gunakan perintah JUAL atau GADAI untuk menjual atau menggadai properti ke Bank!");
        while (!bisaBayarLangsung(p, harga)) {
            OutputHandler::cetakPesan("Uang masih tidak cukup! Gadaikan atau jual lagi propertimu!");
            // menerima command input dari user tapi harus jual atau gadai atau tebus (gunanya tebus buat beli gadai terus dijual beneran, biar totalnya nanti dapet duitnya lebih banyak)

        }

    }
}

// SelesaiGame
Pemain* SelesaiGame::getPemenang (vector<Pemain*> listPemain) {
    Pemain* pemenang = new Pemain("temp", 0);

    for (auto player : listPemain) {
        if (player->getStatus() != StatusPemain::BANKRUPT) {
            if (player > pemenang) {
                pemenang = player;
            }
        }
    }

    return pemenang;
    
}

bool SelesaiGame::sisaSatuOrang (vector<Pemain*> listPemain) {
    int count = 0;

    for (auto player : listPemain) {
        if (player->getStatus() == StatusPemain::ACTIVE) {
            count++;
        }
    }

    return count == 1;
}

void SelesaiGame::tampilPanelSelesaiKarenaMaxTurn (int maxTurn) {
    OutputHandler::cetakPesan("Permainan Selesai! Sudah mencapai " + std::to_string(maxTurn) + " giliran!");
}

void SelesaiGame::tampilPanelSelesaiKarenaBangkrut () {
    OutputHandler::cetakPesan("Permainan Selesai! Semua pemain kecuali satu pemain bangkrut");
}

void SelesaiGame::tampilkanPemenang (Pemain *p) {
    OutputHandler::cetakPesan("Pemenang permainan ini adalah " + p->getUsername());
}
>>>>>>> 4c863580e8432dbd781567a56c63fe50b90b3f39
