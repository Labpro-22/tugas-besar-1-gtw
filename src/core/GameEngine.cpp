#include "../../include/core/GameEngine.hpp"
#include "../../include/utils/NimonspoliException.hpp"
#include "../../include/views/InputHandler.hpp"
#include "../../include/utils/SaveNLoad.hpp"

GameEngine::GameEngine() 
    : configData(nullptr), papanPermainan(nullptr), logGame(nullptr), deckKartu(nullptr), 
      deckSkill(nullptr), dadu(nullptr), managerPenjara(nullptr), managerProperti(nullptr),
      actionService(nullptr), currentTurnIndex(0), maxTurn(0), arahNormalTurn(true) {
}

GameEngine::~GameEngine() {
    // Cleanup jika diperlukan
    if (configData) delete configData;
    for (Pemain* p : listPemain) {
        delete p;
    }
    // urutanPemain berisi pointer yang sama dengan listPemain (bukan owner)
    urutanPemain.clear();
}

void GameEngine::startGame() {
    // Jika game di-load, urutan pemain sudah di-set dari file save.
    // Hanya random saat urutan belum tersedia/invalid.
    if (urutanPemain.size() != listPemain.size() || urutanPemain.empty()) {
        randomizeTurn();
        currentTurnIndex = 0;
    } else if (currentTurnIndex < 0 || currentTurnIndex >= static_cast<int>(urutanPemain.size())) {
        currentTurnIndex = 0;
    }

    if (currentGlobalTurn < 1) {
        currentGlobalTurn = 1;
    }

    auto jalankanTurnPemain = [&](Pemain* player) -> bool {
        if (player->getStatus() == StatusPemain::BANKRUPT) {
            return false;
        }

        if (SelesaiGame::sisaSatuOrang(listPemain)) {
            SelesaiGame::tampilPanelSelesaiKarenaBangkrut();
            SelesaiGame::tampilkanPemenang(player);
            return true;
        }

        bool extraTurn = false;
        int doubleCount = 0;
        do {
            extraTurn = startTurn(player, currentGlobalTurn, doubleCount);

            // Habis turn selesai normalnya player pindah tempat -> jalankan onLanded.
            if (player->getStatus() == StatusPemain::ACTIVE ||
                (player->getStatus() == StatusPemain::JAILED && doubleCount == 3)) {
                Petak* diTempati = papanPermainan->getPetak(static_cast<int>(player->getPosisi()));
                if (diTempati) {
                    diTempati->onLanded(*player, *actionService);
                }
            }

            if (player->getStatus() == StatusPemain::BANKRUPT) {
                break;
            }
        } while (extraTurn && doubleCount < 3);

        return SelesaiGame::sisaSatuOrang(listPemain);
    };

    // Mode bankruptcy (maxTurn invalid): lanjut sampai sisa 1 pemain aktif.
    if (maxTurn < 1) {
        while (true) {
            if (SelesaiGame::sisaSatuOrang(listPemain)) {
                break;
            }

            int pemainAktifPadaRonde = 0;
            for (size_t langkah = 0; langkah < urutanPemain.size(); ++langkah) {
                int idx = (currentTurnIndex + static_cast<int>(langkah)) % static_cast<int>(urutanPemain.size());
                Pemain* player = urutanPemain[idx];
                if (player->getStatus() == StatusPemain::BANKRUPT) {
                    continue;
                }

                pemainAktifPadaRonde++;
                currentTurnIndex = idx;
                if (jalankanTurnPemain(player)) {
                    SelesaiGame::tampilPanelSelesaiKarenaBangkrut();
                    SelesaiGame::tampilkanPemenang(SelesaiGame::getPemenang(listPemain));
                    return;
                }
            }

            if (pemainAktifPadaRonde == 0) {
                break;
            }
            currentGlobalTurn++;
        }

        SelesaiGame::tampilPanelSelesaiKarenaBangkrut();
        SelesaiGame::tampilkanPemenang(SelesaiGame::getPemenang(listPemain));
        return;
    }

    // Mode normal max turn.
    while (currentGlobalTurn <= maxTurn) {
        for (size_t langkah = 0; langkah < urutanPemain.size(); ++langkah) {
            int idx = (currentTurnIndex + static_cast<int>(langkah)) % static_cast<int>(urutanPemain.size());
            Pemain* player = urutanPemain[idx];
            if (player->getStatus() == StatusPemain::BANKRUPT) {
                continue;
            }

            currentTurnIndex = idx;
            if (jalankanTurnPemain(player)) {
                SelesaiGame::tampilPanelSelesaiKarenaBangkrut();
                SelesaiGame::tampilkanPemenang(SelesaiGame::getPemenang(listPemain));
                return;
            }
        }

        currentTurnIndex = (currentTurnIndex + 1) % static_cast<int>(urutanPemain.size());
        currentGlobalTurn++;
    }

    Pemain* pemenang = SelesaiGame::getPemenang(listPemain);
    SelesaiGame::tampilPanelSelesaiKarenaMaxTurn(maxTurn);
    SelesaiGame::tampilkanPemenang(pemenang);
}

void GameEngine::randomizeTurn() {
    urutanPemain = listPemain; // pointer asli, state tetap konsisten
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(urutanPemain.begin(), urutanPemain.end(), gen);
}

bool GameEngine::startTurn(Pemain* p, int currentTurn, int& doubleCount) {
    // Tahap 2: command dispatcher + validasi dasar (skill sebelum dadu)
    bool awalGiliran = (doubleCount == 0);
    bool sudahLemparDadu = false;
    bool extraTurn = false;

    // Awal giliran: reset status dan bagikan 1 kartu skill acak
    if (awalGiliran) {
        p->resetSkillUsage();

        if (deckSkill) {
            try {
                KartuKemampuanSpesial* baru = deckSkill->ambilKartu();
                try {
                    p->tambahKartu(baru, *deckSkill);
                    OutputHandler::cetakEfekKartu("Kartu Skill", "Kamu mendapatkan: " + baru->getDeskripsi());
                } catch (const SlotKartuPenuhException&) {
                    // slot penuh → wajib buang 1 kartu, lalu masukkan kartu baru
                    OutputHandler::cetakError("Kamu sudah memiliki 3 kartu (maksimal). Kamu wajib membuang 1 kartu.");
                    const auto& hand = p->getKartuDiTangan();
                    for (size_t i = 0; i < hand.size(); ++i) {
                        OutputHandler::cetakPesan(std::to_string(i + 1) + ". " + hand[i]->getDeskripsi());
                    }
                    OutputHandler::cetakPesan(std::to_string(hand.size() + 1) + ". " + baru->getDeskripsi() + " (kartu baru)");
                    int pilih = InputHandler::promptAngka("Pilih kartu yang dibuang (1-" + std::to_string(hand.size() + 1) + "): ", 1, hand.size() + 1);
                    if (pilih == static_cast<int>(hand.size() + 1)) {
                        // buang kartu baru
                        deckSkill->buangKartu(baru);
                    } else {
                        p->buangKartu(pilih - 1, *deckSkill);
                        p->tambahKartu(baru, *deckSkill);
                        OutputHandler::cetakEfekKartu("Kartu Skill", "Kamu mendapatkan: " + baru->getDeskripsi());
                    }
                }
            } catch (const std::exception&) {
                // deck kosong → abaikan (akan dihandle DeckKartu::acakUlang)
            }
        }
    }

    if (awalGiliran && p->getStatus() == StatusPemain::JAILED) {
        OutputHandler::cetakPesan("\n=== " + p->getUsername() + " berada di PENJARA ===");
        if (p->getPercobaanKeluarPenjara() >= 3) {
            OutputHandler::cetakPesan("Sudah 3 giliran di penjara. Kamu dipaksa membayar denda!");
            managerPenjara->paksaBayar(*p, *actionService);
        } else {
            OutputHandler::cetakPesan("Kamu memiliki opsi untuk keluar dari penjara:");
            if (configData) OutputHandler::cetakPesan("- Ketik BAYAR_DENDA untuk membayar M" + std::to_string(configData->getDendaPenjara()));
            OutputHandler::cetakPesan("- Ketik GUNAKAN_KEMAMPUAN untuk memakai kartu Bebas Penjara (jika ada)");
            OutputHandler::cetakPesan("- Ketik LEMPAR_DADU / ATUR_DADU untuk mencoba mendapat double.");
        }
    }

    OutputHandler::cetakPesan("\n=== Giliran " + p->getUsername() + " (Turn " + std::to_string(currentTurn) + "/" + std::to_string(maxTurn) + ") ===");

    while (true) {
        try {
            std::string line = InputHandler::promptLine("Perintah> ");
            if (line.empty()) continue;

            // normalisasi sederhana: trim + uppercase + ubah '-' jadi '_' + collapse spasi
            auto trim = [](std::string s) {
                size_t a = s.find_first_not_of(" \t\r\n");
                size_t b = s.find_last_not_of(" \t\r\n");
                if (a == std::string::npos) return std::string();
                return s.substr(a, b - a + 1);
            };
            line = trim(line);
            for (char& c : line) {
                if (c == '-') c = '_';
                c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
            }

            // tokenisasi by space
            std::vector<std::string> tok;
            {
                std::string cur;
                for (char c : line) {
                    if (c == ' ' || c == '\t') {
                        if (!cur.empty()) { tok.push_back(cur); cur.clear(); }
                    } else cur.push_back(c);
                }
                if (!cur.empty()) tok.push_back(cur);
            }
            if (tok.empty()) continue;
            const std::string& cmd = tok[0];

            if (cmd == "CETAK_PAPAN") {
                OutputHandler::cetakPapan(*papanPermainan, listPemain, currentTurn, maxTurn);
                awalGiliran = false;
                continue;
            }

            if (cmd == "CETAK_AKTA") {
                if (tok.size() >= 2) {
                    Petak* petak = papanPermainan->getPetak(tok[1]);
                    auto prop = dynamic_cast<PetakProperti*>(petak);
                    if (!prop) {
                        OutputHandler::cetakError("Kode properti tidak valid!");
                    } else if (configData) {
                        OutputHandler::cetakAktaProperti(prop, *configData);
                    }
                } else {
                    std::string kode = InputHandler::promptString("Masukkan kode petak: ");
                    Petak* petak = papanPermainan->getPetak(kode);
                    auto prop = dynamic_cast<PetakProperti*>(petak);
                    if (!prop) {
                        OutputHandler::cetakError("Kode properti tidak valid!");
                    } else if (configData) {
                        OutputHandler::cetakAktaProperti(prop, *configData);
                    }
                }
                awalGiliran = false;
                continue;
            }

            if (cmd == "CETAK_PROPERTI") {
                if (configData) OutputHandler::cetakPropertiPemain(p, *configData);
                awalGiliran = false;
                continue;
            }

            if (cmd == "CETAK_LOG") {
                int top = -1;
                if (tok.size() >= 2) top = std::stoi(tok[1]);
                if (logGame) {
                    if (top > 0) {
                        auto logs = logGame->getLogs(top);
                        OutputHandler::cetakPesan("=== Log Transaksi (" + std::to_string(top) + " Terakhir) ===");
                        for (const auto& e : logs) {
                            OutputHandler::cetakPesan("[Turn " + std::to_string(e.getRonde()) + "] " + e.getUsername() + " | " + e.getAksi() + " | " + e.getDetail());
                        }
                    } else {
                        OutputHandler::cetakPesan("=== Log Transaksi Penuh ===");
                        for (const auto& e : logGame->getLogs()) {
                            OutputHandler::cetakPesan("[Turn " + std::to_string(e.getRonde()) + "] " + e.getUsername() + " | " + e.getAksi() + " | " + e.getDetail());
                        }
                    }
                }
                awalGiliran = false;
                continue;
            }

            if (cmd == "GADAI") {
                if (sudahLemparDadu) {
                    OutputHandler::cetakError("GADAI hanya bisa dilakukan SEBELUM melempar dadu.");
                    continue;
                }
                if (configData) {
                    actionService->gadaiProperti(*p);
                }
                awalGiliran = false;
                continue;
            }

            if (cmd == "TEBUS") {
                if (configData) {
                    actionService->tebusProperti(*p);
                }
                awalGiliran = false;
                continue;
            }

            if (cmd == "BANGUN") {
                if (configData) {
                    actionService->bangunProperti(*p);
                }
                awalGiliran = false;
                continue;
            }

            if (cmd == "GUNAKAN_KEMAMPUAN") {
                if (sudahLemparDadu) {
                    OutputHandler::cetakError("Kartu kemampuan hanya bisa digunakan SEBELUM melempar dadu.");
                    continue;
                }
                const auto& hand = p->getKartuDiTangan();
                if (hand.empty()) {
                    OutputHandler::cetakError("Kamu tidak punya kartu kemampuan.");
                    continue;
                }
                OutputHandler::cetakPesan("Daftar Kartu Kemampuan Spesial Anda:");
                for (size_t i = 0; i < hand.size(); ++i) {
                    OutputHandler::cetakPesan(std::to_string(i + 1) + ". " + hand[i]->getDeskripsi());
                }
                int pilih = InputHandler::promptAngka("Pilih kartu (1-" + std::to_string(hand.size()) + ", 0 batal): ", 0, hand.size());
                if (pilih == 0) continue;
                p->gunakanKartu(pilih - 1, *actionService, *deckSkill, true);
                awalGiliran = false;
                continue;
            }

            if (cmd == "LEMPAR_DADU") {
                if (sudahLemparDadu) {
                    OutputHandler::cetakError("Kamu sudah melempar dadu di giliran ini.");
                    continue;
                }
                
                dadu->rollRandom();
                OutputHandler::cetakPesan("Hasil: " + std::to_string(dadu->getNilaid1()) + " + " + std::to_string(dadu->getNilaid2()) + " = " + std::to_string(dadu->getTotalNilaiDadu()));
                
                bool isDouble = dadu->isDouble();

                if (p->getStatus() == StatusPemain::JAILED) {
                    if (isDouble) {
                        OutputHandler::cetakPesan("Double! Kamu berhasil keluar dari penjara.");
                        p->setStatus(StatusPemain::ACTIVE);
                        p->resetPercobaanPenjara();
                        actionService->movePlayerRelative(*p, dadu->getTotalNilaiDadu());
                        extraTurn = false; 
                    } else {
                        OutputHandler::cetakPesan("Bukan double. Kamu tetap di penjara.");
                        p->tambahPercobaanPenjara();
                        extraTurn = false;
                    }
                    sudahLemparDadu = true;
                    break;
                }

                if (isDouble) {
                    doubleCount++;
                    OutputHandler::cetakPesan("[DOUBLE] " + std::to_string(doubleCount) + "x berturut-turut!");
                    if (doubleCount == 3) {
                        OutputHandler::cetakPesan("3x double! Kamu melanggar batas kecepatan dan masuk penjara!");
                        actionService->sendToJail(*p);
                        extraTurn = false;
                        sudahLemparDadu = true;
                        break;
                    }
                    extraTurn = true;
                } else {
                    extraTurn = false;
                }

                actionService->movePlayerRelative(*p, dadu->getTotalNilaiDadu());
                sudahLemparDadu = true;
                break;
            }

            if (cmd == "ATUR_DADU") {
                if (sudahLemparDadu) {
                    OutputHandler::cetakError("Kamu sudah melempar dadu di giliran ini.");
                    continue;
                }
                if (tok.size() < 3) {
                    OutputHandler::cetakError("Format: ATUR_DADU X Y");
                    continue;
                }
                int x = std::stoi(tok[1]);
                int y = std::stoi(tok[2]);
                if (x < 1 || x > 6 || y < 1 || y > 6) {
                    OutputHandler::cetakError("Nilai dadu harus 1-6.");
                    continue;
                }

                dadu->rollManual(x, y);
                OutputHandler::cetakPesan("Hasil: " + std::to_string(x) + " + " + std::to_string(y) + " = " + std::to_string(dadu->getTotalNilaiDadu()));
                
                bool isDouble = dadu->isDouble();

                if (p->getStatus() == StatusPemain::JAILED) {
                    if (isDouble) {
                        OutputHandler::cetakPesan("Double! Kamu berhasil keluar dari penjara.");
                        p->setStatus(StatusPemain::ACTIVE);
                        p->resetPercobaanPenjara();
                        actionService->movePlayerRelative(*p, dadu->getTotalNilaiDadu());
                        extraTurn = false; 
                    } else {
                        OutputHandler::cetakPesan("Bukan double. Kamu tetap di penjara.");
                        p->tambahPercobaanPenjara();
                        extraTurn = false;
                    }
                    sudahLemparDadu = true;
                    break;
                }

                if (isDouble) {
                    doubleCount++;
                    OutputHandler::cetakPesan("[DOUBLE] " + std::to_string(doubleCount) + "x berturut-turut!");
                    if (doubleCount == 3) {
                        OutputHandler::cetakPesan("3x double! Kamu melanggar batas kecepatan dan masuk penjara!");
                        actionService->sendToJail(*p);
                        extraTurn = false;
                        sudahLemparDadu = true;
                        break;
                    }
                    extraTurn = true;
                } else {
                    extraTurn = false;
                }

                actionService->movePlayerRelative(*p, dadu->getTotalNilaiDadu());
                sudahLemparDadu = true;
                break;
            }

            if (cmd == "BAYAR_DENDA") {
                if (p->getStatus() == StatusPemain::JAILED) {
                    if (configData && p->getSaldo() >= configData->getDendaPenjara()) {
                        managerPenjara->escapeByFine(*p, *actionService);
                        // Giliran berlanjut (boleh lempar dadu)
                    } else {
                        OutputHandler::cetakError("Uang tidak cukup untuk bayar denda penjara.");
                    }
                } else {
                    OutputHandler::cetakError("Kamu tidak sedang dipenjara.");
                }
                continue;
            }

            if (cmd == "SIMPAN") {
                if (!awalGiliran) {
                    OutputHandler::cetakError("Tidak bisa menyimpan game di tengah-tengah giliran!");
                    continue;
                }
                if (tok.size() < 2) {
                    OutputHandler::cetakError("Format: SIMPAN <filename>");
                    continue;
                }
                SaveNLoad snl;
                snl.saveGameState(*this, tok[1]);
                OutputHandler::cetakPesan("Permainan berhasil disimpan ke: " + tok[1]);
                continue;
            }

            if (cmd == "BANTUAN" || cmd == "HELP") {
                OutputHandler::cetakPesan("Perintah tersedia: CETAK_PAPAN, CETAK_PROPERTI, CETAK_AKTA, CETAK_LOG [N], GADAI, TEBUS, BANGUN, GUNAKAN_KEMAMPUAN, LEMPAR_DADU, ATUR_DADU X Y, BAYAR_DENDA, SIMPAN <file>");
                continue;
            }

            throw PerintahTidakDitemukanException();
        } catch (const NimonspoliException& e) {
            OutputHandler::cetakError(e.what());
            OutputHandler::cetakPesan("Silakan coba perintah lain.");
        } catch (const std::exception& e) {
            OutputHandler::cetakError(e.what());
        }
    }
    return extraTurn;
}
