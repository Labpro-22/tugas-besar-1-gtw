#include "utils/SaveNLoad.hpp"
#include <sstream>
#include <fstream>
#include "utils/FileHelper.hpp"
#include "utils/NimonspoliException.hpp"
#include "core/GameEngine.hpp"
#include "models/Petak/PetakProperti.hpp"
#include "models/Petak/PetakLahan.hpp"
#include "models/Petak/Petak.hpp"
#include "models/Kartu/KartuKemampuanSpesial.hpp"
#include "models/Kartu/DeckKartu.hpp"
#include "models/Managers/ManagerFestival.hpp"

using namespace std;

void SaveNLoad::loadGameState(GameEngine& game, string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw FileTidakValidException(); // File tidak ditemukan
    }

    string line;
    
    // <TURN_SAAT_INI> <MAX_TURN>
    if (!getline(file, line)) throw FileTidakValidException(); // File rusak atau format tidak dikenali
    stringstream ss(line);
    int currentGlobalTurn, maxTurn;
    ss >> currentGlobalTurn >> maxTurn;
    game.setCurrentGlobalTurn(currentGlobalTurn);
    game.setMaxTurn(maxTurn);

    // <JUMLAH_PEMAIN>
    if (!getline(file, line)) throw FileTidakValidException(); // File rusak atau format tidak dikenali
    int numPemain = stoi(line);

    vector<Pemain*> loadedPlayers;
    for (int i = 0; i < numPemain; i++) {
        if (!getline(file, line)) throw FileTidakValidException(); // File rusak atau format tidak dikenali
        stringstream pss(line);
        string username, kodePetak, statusStr;
        int uang;
        pss >> username >> uang >> kodePetak >> statusStr;

        Pemain* p = new Pemain(username, uang);
        
        // Status
        if (statusStr == "BANKRUPT") p->setStatus(StatusPemain::BANKRUPT);
        else if (statusStr == "JAILED") p->setStatus(StatusPemain::JAILED);
        else p->setStatus(StatusPemain::ACTIVE);

        // Posisi berdasarkan kode petak di papan
        if (game.getPapanPermainan()) {
            Petak* petak = game.getPapanPermainan()->getPetak(kodePetak);
            if (petak) p->setPosisi(petak->getIndeks());
        }

        // <JUMLAH_KARTU_TANGAN>
        if (!getline(file, line)) throw FileTidakValidException(); // File rusak atau format tidak dikenali
        int numCards = stoi(line);
        for (int j = 0; j < numCards; j++) {
            if (!getline(file, line)) throw FileTidakValidException(); // File rusak atau format tidak dikenali
            stringstream css(line);
            string jenisKartu;
            css >> jenisKartu;
            
            // Factory sederhana untuk memuat kartu dari tipe string
            KartuKemampuanSpesial* kartu = nullptr;
            if (jenisKartu == "MoveCard") {
                int langkah = 1;
                if (css >> langkah) kartu = new MoveCard(langkah);
                else kartu = new MoveCard(1); // fallback
            } else if (jenisKartu == "DiscountCard") {
                int persen = 30;
                int durasi = 1;
                css >> persen >> durasi;
                auto dc = new DiscountCard(persen);
                dc->setSisaDurasi(durasi);
                kartu = dc;
            } else if (jenisKartu == "ShieldCard") {
                kartu = new ShieldCard();
            } else if (jenisKartu == "TeleportCard") {
                kartu = new TeleportCard();
            } else if (jenisKartu == "LassoCard") {
                kartu = new LassoCard();
            } else if (jenisKartu == "DemolitionCard") {
                kartu = new DemolitionCard();
            } else if (jenisKartu == "RotasiKartuCard") {
                kartu = new RotasiKartuCard();
            } else if (jenisKartu == "ReverseCard") {
                kartu = new ReverseCard();
            } else if (jenisKartu == "PenjaraKanCard") {
                kartu = new PenjaraKanCard();
            }

            if (kartu) {
                // Data save dianggap valid → bypass tambahKartu() (yang butuh deckSkill)
                // Gunakan setKartuDiTangan() untuk memasukkan kartu yang dimuat
                auto hand = p->getKartuDiTangan();
                hand.push_back(kartu);
                p->setKartuDiTangan(hand);
            }
        }
        loadedPlayers.push_back(p);
    }

    game.setListPemain(loadedPlayers);

    // <URUTAN_GILIRAN_1> ...
    if (!getline(file, line)) throw FileTidakValidException(); // File rusak atau format tidak dikenali
    stringstream urutss(line);
    string uname;
    vector<Pemain*> urutanPemain;
    while (urutss >> uname) {
        for (Pemain* p : loadedPlayers) {
            if (p->getUsername() == uname) {
                urutanPemain.push_back(p);
                break;
            }
        }
    }
    game.setUrutanPemain(urutanPemain);

    // <GILIRAN_AKTIF_SAAT_INI>
    if (!getline(file, line)) throw FileTidakValidException(); // File rusak atau format tidak dikenali
    string aktifUname = line;
    int curIdx = 0;
    for (size_t i = 0; i < urutanPemain.size(); ++i) {
        if (urutanPemain[i]->getUsername() == aktifUname) {
            curIdx = i;
            break;
        }
    }
    game.setCurrentTurnIndex(curIdx);

    // <STATE_PROPERTI>
    if (!getline(file, line)) throw FileTidakValidException(); // File rusak atau format tidak dikenali
    int numProperti = stoi(line);
    
    for (int i = 0; i < numProperti; i++) {
        if (!getline(file, line)) throw FileTidakValidException(); // File rusak atau format tidak dikenali
        stringstream propss(line);
        string kode, jenis, pemilikStr, statusStr, nBangunanStr;
        int fmult, fdur;
        propss >> kode >> jenis >> pemilikStr >> statusStr >> fmult >> fdur >> nBangunanStr;

        if (game.getPapanPermainan()) {
            Petak* petak = game.getPapanPermainan()->getPetak(kode);
            if (auto prop = dynamic_cast<PetakProperti*>(petak)) {
                
                if (statusStr == "OWNED") prop->setStatus(PetakProperti::StatusProperti::OWNED);
                else if (statusStr == "MORTGAGED") prop->setStatus(PetakProperti::StatusProperti::MORTGAGED);
                else prop->setStatus(PetakProperti::StatusProperti::BANK);

                if (pemilikStr != "BANK") {
                    Pemain* propOwner = nullptr;
                    for (Pemain* p : loadedPlayers) {
                        if (p->getUsername() == pemilikStr) {
                            propOwner = p;
                            break;
                        }
                    }
                    if (propOwner) {
                        prop->setPemilik(propOwner);
                        propOwner->tambahAset(prop);
                    }
                }

                // Festival
                if (game.getManagerFestival() && fdur > 0) {
                    // Karena ManagerFestival menggunakan add/apply manual, kita bypass langsung
                    // Tapi karena ManagerFestival butuh dipanggil applyFestival, dan kita mau set durasi spesifik:
                    // Untuk saat ini stub:
                    game.getManagerFestival()->setEfekFestival(prop, fmult, fdur);
                }

                // Bangunan
                if (auto lahan = dynamic_cast<PetakLahan*>(prop)) {
                    if (nBangunanStr == "H") {
                        lahan->setJumlahBangunan(5); // Hotel = level 5
                    } else {
                        int nb = stoi(nBangunanStr);
                        while (lahan->getJumlahBangunan() < nb && !lahan->punyaHotel()) {
                        }
                    }
                }
            }
        }
    }

    // <STATE_DECK>
    if (!getline(file, line)) throw FileTidakValidException(); // File rusak atau format tidak dikenali
    int numDeck = stoi(line);
    // Untuk saat ini kita lewatkan saja karena instansiasi deck butuh pointer Kartu asli
    for (int i = 0; i < numDeck; i++) {
        getline(file, line);
    }

    // <STATE_LOG>
    if (!getline(file, line)) throw FileTidakValidException(); // File rusak atau format tidak dikenali
    int numLogs = stoi(line);
    if (game.getLogGame()) {
        for (int i = 0; i < numLogs; i++) {
            if (!getline(file, line)) throw FileTidakValidException(); // File rusak atau format tidak dikenali
            stringstream logss(line);
            int turnLog;
            string uNameLog, aksiLog, detailLog, temp;
            logss >> turnLog >> uNameLog >> aksiLog;
            
            // Baca sisa line sebagai detail
            getline(logss, detailLog);
            if (!detailLog.empty() && detailLog[0] == ' ') {
                detailLog = detailLog.substr(1);
            }

            LogTransaksiEntry entry(turnLog, uNameLog, aksiLog, detailLog);
            game.getLogGame()->tambahLog(entry);
        }
    }
}

void SaveNLoad::saveGameState(GameEngine& game, string filename) {
    std::vector<std::string> lines;

    // <TURN_SAAT_INI> <MAX_TURN>
    // Asumsi GameEngine punya getCurrentGlobalTurn(), untuk sementara pakai 1 jika belum ada
    int currentTurn = game.getCurrentGlobalTurn(); 
    int maxTurn = game.getMaxTurn();
    lines.push_back(to_string(currentTurn) + " " + to_string(maxTurn));

    // <JUMLAH_PEMAIN>
    auto& listPemain = game.getListPemain();
    lines.push_back(to_string(listPemain.size()));

    // <STATE_PEMAIN_1> ...
    for (Pemain* p : listPemain) {
        // <USERNAME> <UANG> <POSISI_PETAK> <STATUS>
        string statusStr = "ACTIVE";
        if (p->getStatus() == StatusPemain::BANKRUPT) statusStr = "BANKRUPT";
        else if (p->getStatus() == StatusPemain::JAILED) statusStr = "JAILED";

        string kodePetak = "START";
        if (game.getPapanPermainan()) {
            Petak* petak = game.getPapanPermainan()->getPetak(p->getPosisi());
            if (petak) kodePetak = petak->getKode();
        }

        lines.push_back(p->getUsername() + " " + to_string(p->getSaldo()) + " " + kodePetak + " " + statusStr);

        // <JUMLAH_KARTU_TANGAN>
        auto& kartuTangan = p->getKartuDiTangan();
        lines.push_back(to_string(kartuTangan.size()));

        // <JENIS_KARTU> <NILAI_KARTU>
        for (auto* kartu : kartuTangan) {
            string namaKartu = kartu->getNamaKartu();
            string nilaiKartu = "";
            
            // Format spesifik nilai kartu jika ada
            if (namaKartu == "MoveCard") {
                // Asumsi bisa mendapatkan langkah dari kartu, untuk stub pakai 1
                nilaiKartu = " 1"; // Harus diakses dari getter jika ada
            } else if (namaKartu == "DiscountCard") {
                nilaiKartu = " 30"; // Stub
            }
            // Karena spesifikasi meminta nilai dikosongkan jika tidak ada, 
            // formatnya adalah string kosong jika tidak ada nilai.
            lines.push_back(namaKartu + nilaiKartu);
        }
    }

    // <URUTAN_GILIRAN_1> ...
    auto& urutanPemain = game.getUrutanPemain();
    string urutanStr = "";
    for (size_t i = 0; i < urutanPemain.size(); ++i) {
        urutanStr += urutanPemain[i]->getUsername() + (i == urutanPemain.size() - 1 ? "" : " ");
    }
    lines.push_back(urutanStr);

    // <GILIRAN_AKTIF_SAAT_INI>
    int curIdx = game.getCurrentTurnIndex();
    if (curIdx >= 0 && curIdx < static_cast<int>(urutanPemain.size())) {
        lines.push_back(urutanPemain[curIdx]->getUsername());
    } else {
        lines.push_back(urutanPemain.empty() ? "None" : urutanPemain[0]->getUsername());
    }

    // <STATE_PROPERTI>
    // Asumsi papanPermainan menyimpan list semua petak. Kita hitung jumlah properti dulu
    std::vector<PetakProperti*> semuaProperti;
    if (game.getPapanPermainan()) {
        for (Petak* petak : game.getPapanPermainan()->getDaftarPetak()) {
            if (auto prop = dynamic_cast<PetakProperti*>(petak)) {
                semuaProperti.push_back(prop);
            }
        }
    }
    lines.push_back(to_string(semuaProperti.size()));

    for (PetakProperti* prop : semuaProperti) {
        string kode = prop->getKode();
        string jenis = prop->getJenisString();
        string pemilik = prop->getPemilik() ? prop->getPemilik()->getUsername() : "BANK";
        
        string statusProp = "BANK";
        if (prop->getStatus() == PetakProperti::StatusProperti::OWNED) statusProp = "OWNED";
        else if (prop->getStatus() == PetakProperti::StatusProperti::MORTGAGED) statusProp = "MORTGAGED";

        int fmult = 1;
        int fdur = 0;
        if (game.getManagerFestival()) {
            if (game.getManagerFestival()->isAktif(prop)) {
                fmult = game.getManagerFestival()->getPengali(prop);
                fdur = game.getManagerFestival()->getSisaDurasi(prop);
            }
        }

        string nBangunan = "0";
        if (auto lahan = dynamic_cast<PetakLahan*>(prop)) {
            if (lahan->punyaHotel()) nBangunan = "H";
            else nBangunan = to_string(lahan->getJumlahBangunan());
        }

        // <KODE_PETAK> <JENIS> <PEMILIK> <STATUS> <FMULT> <FDUR> <N_BANGUNAN>
        // Gunakan padding statis seperti contoh atau spasi saja (spesifikasi biasanya bisa parse spasi berapapun)
        lines.push_back(kode + " " + jenis + " " + pemilik + " " + statusProp + " " + to_string(fmult) + " " + to_string(fdur) + " " + nBangunan);
    }

    // <STATE_DECK>
    // Asumsi Deck Skill ada
    if (game.getDeckKartu() && false) { // Skip dulu sementara tidak ada DeckKartuKemampuanSpesial
        // ...
    } else {
        lines.push_back("0");
    }

    // <STATE_LOG>
    if (game.getLogGame()) {
        auto& log = game.getLogGame()->getLogs();
        lines.push_back(to_string(log.size()));
        for (auto& entry : log) {
            lines.push_back(to_string(entry.getRonde()) + " " + entry.getUsername() + " " + entry.getAksi() + " " + entry.getDetail());
        }
    } else {
        lines.push_back("0");
    }

    FileHelper::writeAllLines(filename, lines);
}
