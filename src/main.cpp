#include <iostream>
#include <string>
#include <vector>

#include "core/GameEngine.hpp"
#include "utils/ConfigLoader.hpp"
#include "utils/NimonspoliException.hpp"
#include "utils/SaveNLoad.hpp"
#include "utils/LogTransaksiGame.hpp"
#include "models/Pemain.hpp"
#include "models/Papan.hpp"
#include "models/Dadu.hpp"
#include "models/Kartu/DeckFactory.hpp"
#include "models/Kartu/DeckKartu.hpp"
#include "models/Kartu/Kartu.hpp"
#include "models/Kartu/KartuKemampuanSpesial.hpp"
#include "models/Managers/ManagerFestival.hpp"
#include "models/Managers/ManagerProperti.hpp"
#include "models/Managers/ManagerPenjara.hpp"
#include "models/Managers/ManagerTransaksi.hpp"
#include "models/Managers/ManagerLelang.hpp"
#include "views/InputHandler.hpp"

using namespace std;

int main() {
    try {
        std::cout << "=== NIMONSPOLI ===\n";
        std::cout << "1. Load Game\n";
        std::cout << "2. New Game\n";

        int pilihan = InputHandler::promptAngka("Pilih (1/2): ", 1, 2);

        GameEngine engine;

        // --- Load konfigurasi ---
        ConfigData* config = new ConfigData(ConfigLoader::loadAll("config"));
        engine.setConfigData(config);
        engine.setMaxTurn(config->getMaxTurn());

        // --- Logger & deck kartu ---
        auto* logger = new LogTransaksiGame();
        engine.setLogGame(logger);

        auto* deckKesempatan = new DeckKartu<Kartu>();
        auto* deckDanaUmum   = new DeckKartu<Kartu>();
        auto* deckSkill      = new DeckKartu<KartuKemampuanSpesial>();
        isiDeckKesempatan(*deckKesempatan);
        isiDeckDanaUmum(*deckDanaUmum);
        isiDeckSkill(*deckSkill, true);
        engine.setDeckSkill(deckSkill);

        // --- Manager & papan ---
        auto* managerFestival = new ManagerFestival();
        auto* managerProperti = new ManagerProperti(*config);
        auto* papan = new Papan(*managerProperti, *config, managerFestival, deckKesempatan, deckDanaUmum);
        engine.setPapanPermainan(papan);
        engine.setManagerProperti(managerProperti);
        engine.setManagerFestival(managerFestival);

        // --- Pemain ---
        if (pilihan == 2) {
            int n = InputHandler::promptAngka("Jumlah pemain (2-4): ", 2, 4);
            std::vector<Pemain*> pemain;
            pemain.reserve(n);
            for (int i = 0; i < n; i++) {
                std::string uname = InputHandler::promptString("Masukkan username pemain " + std::to_string(i+1) + ": ");
                Pemain* p = new Pemain(uname, config->getUangAwalPemain());
                p->setPosisi(1); // start di GO (indeks 1)
                pemain.push_back(p);
            }
            engine.setListPemain(pemain);
        } else {
            std::string fname = InputHandler::promptString("Masukkan nama file save: ");
            SaveNLoad snl;
            snl.loadGameState(engine, fname);
        }

        // --- Service transaksi / penjara / aksi pemain ---
        auto* dadu = new Dadu();
        engine.setDadu(dadu);

        auto* managerPenjara = new ManagerPenjara();
        engine.setManagerPenjara(managerPenjara);

        auto* managerTransaksi = new ManagerTransaksi(logger, &engine.getListPemain());
        (void)ManagerLelang(managerTransaksi); // dipakai penuh saat mekanisme BELI/LELANG aktif

        int& turnIdxRef = engine.getCurrentTurnIndexRef();
        auto* svc = new PlayerActionService(
            nullptr,
            managerPenjara,
            managerProperti,
            managerFestival,
            managerTransaksi,
            config,
            papan,
            logger,
            &engine.getListPemain(),
            &turnIdxRef
        );
        svc->setDadu(dadu);
        int& globalTurnRef = engine.getCurrentGlobalTurnRef();
        svc->setCurrentGlobalTurnRef(&globalTurnRef);
        engine.setActionService(svc);

        engine.startGame();
        return 0;
    } catch (const std::exception& e) {
        std::cout << "[FATAL] " << e.what() << "\n";
        return 1;
    }
}