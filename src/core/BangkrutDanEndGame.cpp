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
