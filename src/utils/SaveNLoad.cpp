#include "include/utils/SaveNLoad.hpp"
#include <sstream>
#include "include/utils/FileHelper.hpp"
#include "include/utils/NimonspoliException.hpp"

using namespace std;

vector<string> SaveNLoad::saveStatePemain(GameEngine& game) {
    vector<string> hasil;
    vector<Pemain*> listPemain = game.getListPemain();
    for (Pemain* pemain : listPemain) {
        if (pemain == nullptr) {
            continue;
        }
        ostringstream builder;
        //<USERNAME> <UANG> <POSISI_PETAK> <STATUS>
        builder << pemain->getUsername() << " "
                << pemain->getSaldo() << " "
                << pemain->getPosisi() << " "
                << pemain->getStatusString();
        // <JUMLAH_KARTU_TANGAN>
        const vector<KartuKemampuanSpesial*>& kartuDiTangan = pemain->getKartuDiTangan();
        builder << "\n" << kartuDiTangan.size();
        // <JENIS_KARTU_1> <NILAI_KARTU_1> <SISA_DURASI_1>
        // .....
        for (const KartuKemampuanSpesial* kartu : kartuDiTangan) {
            if (kartu == nullptr) {
                builder << "\nUNKNOWN  ";
                continue;
            }
            builder << "\n" << kartu->getNamaKartu() << " "
                    << kartu->getNilaiState() << " "
                    << kartu->getSisaDurasiState();
        }
        hasil.push_back(builder.str());
    }

    return hasil;
}

vector<string> SaveNLoad::saveStateGiliran(GameEngine& game){
    vector<string> hasil;
    ostringstream builderUrutan;
    vector<Pemain*> urutanGiliran = game.getUrutanGiliran();
    for (Pemain* pemain : urutanGiliran){
        if (pemain == nullptr) {
            continue;
        }
        // <URUTAN_GILIRAN_1> <URUTAN_GILIRAN_2> … <URUTAN_GILIRAN_N>
        builderUrutan << pemain->getUsername() << " ";
    }
    // <GILIRAN_AKTIF_SAAT_INI>
    ostringstream builderAktif;
    builderAktif << game.getGiliranAktif().getUsername();

    hasil.push_back(builderUrutan.str());
    hasil.push_back(builderAktif.str());

    return hasil;
}

vector<string> SaveNLoad::saveStateProperti(GameEngine& game){
    vector<string> hasil;
    // <JUMLAH_PROPERTI>
    ostringstream builderJumlahProp;
    builderJumlahProp << game.getjumlahProperti();
    hasil.push_back(builderJumlahProp.str());
    // <KODE_PETAK> <JENIS> <PEMILIK> <STATUS> <FMULT> <FDUR> <N_BANGUNAN>
    const auto& daftarProperti = game.getDaftarProperti();
    for (const auto& entry : daftarProperti) {
        Properti* properti = entry.second;
        if (properti == nullptr) {
            continue;
        }
        ostringstream builder;
        builder << properti->getKode() << " "
                << game.getJenisPropertiString(properti) << " "
                << game.getPemilikPropertiString(properti) << " "
                << game.getStatusPropertiString(properti) << " "
                << game.getFestivalPropertiString(properti) << " "
                << game.getBangunanPropertiString(properti);

        hasil.push_back(builder.str());
    }

    return hasil;
}

vector<string> SaveNLoad::saveStateDeck(GameEngine& game){
    //<JUMLAH_KARTU_DECK_KEMAMPUAN>
    vector<string> hasil;
    ostringstream builderJumlahDeck;
    builderJumlahDeck << game.getJumlahKartuDeckKemampuan();
    hasil.push_back(builderJumlahDeck.str());

    //<JENIS_KARTU_1>
    //...
    const vector<KartuKemampuanSpesial*>& isiDeck = game.getIsiDeckKemampuan();
    for (const KartuKemampuanSpesial* kartu : isiDeck) {
        if (kartu == nullptr) {
            hasil.push_back("UNKNOWN");
            continue;
        }

        hasil.push_back(kartu->getNamaKartu());
    }

    return hasil;
}

vector<string> SaveNLoad::saveStateLog(GameEngine& game){
    vector<string> hasil;

    // <JUMLAH_ENTRI_LOG>
    ostringstream builderJumlahLog;
    builderJumlahLog << game.getJumlahEntriLog();
    hasil.push_back(builderJumlahLog.str());

    // <TURN> <USERNAME> <JENIS_AKSI> <DETAIL>
    // …
    const vector<LogTransaksiEntry>& entriLog = game.getEntriLog();
    for (const LogTransaksiEntry& entry : entriLog) {
        ostringstream builder;
        builder << entry.getRonde() << " "
                << entry.getUsername() << " "
                << entry.getAksi() << " "
                << entry.getDetail();

        hasil.push_back(builder.str());
    }

    return hasil;

}

void SaveNLoad::saveGameState(GameEngine game, string filename){
    vector<string> lines;

    if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt") {
        filename += ".txt";
    }
    // <TURN_SAAT_INI> <MAX_TURN>
    lines.push_back(to_string(game.getTurnSaatIni()) + " " + to_string(game.getMaxTurn()));
    // <JUMLAH_PEMAIN>
    lines.push_back(to_string(game.getJumlahPemain()));

    auto tulisSection = [&lines](const vector<string>& section) {
        for (const string& baris : section) {
            lines.push_back(baris);
        }
    };
    // <STATE_PEMAIN_1> ... <STATE_PEMAIN_N>
    vector<string> statePemain = saveStatePemain(game);
    tulisSection(statePemain);
    // <URUTAN_GILIRAN_1> ... dan <GILIRAN_AKTIF_SAAT_INI>
    vector<string> stateGiliran = saveStateGiliran(game);
    tulisSection(stateGiliran);
    // <STATE_PROPERTI>
    vector<string> stateProperti = saveStateProperti(game);
    tulisSection(stateProperti);
    // <STATE_DECK>
    vector<string> stateDeck = saveStateDeck(game);
    tulisSection(stateDeck);
    // <STATE_LOG>
    vector<string> stateLog = saveStateLog(game);
    tulisSection(stateLog);

    FileHelper::writeAllLines(filename, lines);
}
