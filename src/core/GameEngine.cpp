#include "../../include/core/GameEngine.hpp"
#include "../../include/core/TurnCommandProcessor.hpp"
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
                bool arahMaju = actionService ? actionService->isArahNormal() : true;
                int idx = 0;
                if (arahMaju) {
                    idx = (currentTurnIndex + static_cast<int>(langkah)) % static_cast<int>(urutanPemain.size());
                } else {
                    idx = (currentTurnIndex - static_cast<int>(langkah)) % static_cast<int>(urutanPemain.size());
                    if (idx < 0) idx += static_cast<int>(urutanPemain.size());
                }
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
            bool arahMaju = actionService ? actionService->isArahNormal() : true;
            if (arahMaju) {
                currentTurnIndex = (currentTurnIndex + 1) % static_cast<int>(urutanPemain.size());
            } else {
                currentTurnIndex = (currentTurnIndex - 1) % static_cast<int>(urutanPemain.size());
                if (currentTurnIndex < 0) currentTurnIndex += static_cast<int>(urutanPemain.size());
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
            bool arahMaju = actionService ? actionService->isArahNormal() : true;
            int idx = 0;
            if (arahMaju) {
                idx = (currentTurnIndex + static_cast<int>(langkah)) % static_cast<int>(urutanPemain.size());
            } else {
                idx = (currentTurnIndex - static_cast<int>(langkah)) % static_cast<int>(urutanPemain.size());
                if (idx < 0) idx += static_cast<int>(urutanPemain.size());
            }
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

        bool arahMaju = actionService ? actionService->isArahNormal() : true;
        if (arahMaju) {
            currentTurnIndex = (currentTurnIndex + 1) % static_cast<int>(urutanPemain.size());
        } else {
            currentTurnIndex = (currentTurnIndex - 1) % static_cast<int>(urutanPemain.size());
            if (currentTurnIndex < 0) currentTurnIndex += static_cast<int>(urutanPemain.size());
        }
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
    TurnCommandState state;
    state.awalGiliran = (doubleCount == 0);
    state.doubleCount = &doubleCount;

    // Awal giliran: reset status dan bagikan 1 kartu skill acak
    if (state.awalGiliran) {
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

    if (state.awalGiliran && p->getStatus() == StatusPemain::JAILED) {
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
    TurnCommandProcessor commandProcessor(
        *this,
        configData,
        papanPermainan,
        listPemain,
        logGame,
        deckSkill,
        dadu,
        managerPenjara,
        actionService,
        maxTurn
    );

    while (true) {
        try {
            bool shouldEndTurn = commandProcessor.processNextCommand(p, currentTurn, state);
            if (shouldEndTurn) {
                break;
            }
        } catch (const NimonspoliException& e) {
            OutputHandler::cetakError(e.what());
            OutputHandler::cetakPesan("Silakan coba perintah lain.");
        } catch (const std::exception& e) {
            OutputHandler::cetakError(e.what());
        }
    }
    return state.extraTurn;
}
