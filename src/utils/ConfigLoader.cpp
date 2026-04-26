#include "utils/ConfigLoader.hpp"
#include "utils/FileHelper.hpp"
#include "utils/NimonspoliException.hpp"
#include <sstream>
#include <utility>

using namespace std;

ConfigData ConfigLoader::loadAll(const string& folderPath){
    auto prop = loadProperty(FileHelper::readAllLines(folderPath + "/property.txt"));
    auto aksi = loadAksi(FileHelper::readAllLines(folderPath + "/aksi.txt"));
    auto rr = loadRailroad(FileHelper::readAllLines(folderPath + "/railroad.txt"));
    auto util = loadUtility(FileHelper::readAllLines(folderPath + "/utility.txt"));
    auto tax = loadTax(FileHelper::readAllLines(folderPath + "/tax.txt"));
    auto sp = loadSpecial(FileHelper::readAllLines(folderPath + "/special.txt"));
    auto misc = loadMisc(FileHelper::readAllLines(folderPath + "/misc.txt"));

    return ConfigData(tax.pphFlat, tax.pphPersen, tax.pbmFlat, sp.gajiGo, sp.dendaPenjara,
                      misc.maxTurn, misc.uangAwal, move(prop), move(aksi), move(rr), move(util));
}

map<int, PropertiConfig> ConfigLoader::loadProperty(const vector<string>& lines){
    map<int, PropertiConfig> propMap;
   
    for (const string& line : lines) {
        if (line.empty()) {
            continue;
        }

        istringstream iss(line);
        int id;
        string kode;
        string nama;
        string jenis;
        string warna;
        
        if (!(iss >> id >> kode >> nama >> jenis >> warna)) {
            continue;
        }

        if (jenis == "STREET") {
            int hargaLahan, nilaiGadai, upgRumah, upgHotel;
            vector<int> rent(6);
            
            if (!(iss >> hargaLahan >> nilaiGadai >> upgRumah >> upgHotel
                      >> rent[0] >> rent[1] >> rent[2] >> rent[3] >> rent[4] >> rent[5])) {
                continue;
            }
            
            PropertiConfig prop(id, kode, nama, jenis, warna, hargaLahan, nilaiGadai, upgRumah, upgHotel, rent);
            propMap[id] = prop;
            
        } else if (jenis == "RAILROAD" || jenis == "UTILITY") {
            // Untuk railroad gada harga upgrade dan hargaSewa handled via railroad.txt
            // Begitu juga untuk utility gada harga upgrade dan hargaSewa tergantung faktor pengali handled via utility.txt
            int hargaLahan, nilaiGadai;
            
            if (!(iss >> hargaLahan >> nilaiGadai)) {
                continue;
            }
            vector<int> rent(6, 0); //0 0 0 0 0 0
            PropertiConfig prop(id, kode, nama, jenis, warna, hargaLahan, nilaiGadai, 0, 0, rent);
            propMap[id] = prop;
            
        } else {
            continue;
        }
    }

    //handle kalau kosong
    if (propMap.empty()) {
        throw FileTidakValidException();
    }

    return propMap;
}

map<int, int> ConfigLoader::loadRailroad(const vector<string>& lines) {
    map<int, int> result;

    for (const string& line : lines) {
        if (line.empty()) {
            continue;
        }

        istringstream iss(line);
        int jumlah;
        int biaya;
        if (!(iss >> jumlah >> biaya)) {
            continue;
        }
        result[jumlah] = biaya;
    }

    //handle kalau kosong
    if (result.empty()) {
        throw FileTidakValidException();
    }

    //handle key railroad harus berurutan mulai dari 1 sampai n 
    int expected = 1;
    for (const auto& entry : result) {
        if (entry.first != expected) {
            throw FileTidakValidException();
        }
        expected++;
    }

    //sewa tidak boleh negatif.
    for (const auto& entry : result) {
        if (entry.second < 0) {
            throw FileTidakValidException();
        }
    }

    return result;
}

map<int, int> ConfigLoader::loadUtility(const vector<string>& lines) {
    map<int, int> result;

    for (const string& line : lines) {
        if (line.empty()) {
            continue;
        }

        istringstream iss(line);
        int jumlah;
        int pengali;
        if (!(iss >> jumlah >> pengali)) {
            continue;
        }
        result[jumlah] = pengali;
    }
    //handle kalau kosong
    if (result.empty()) {
        throw FileTidakValidException();
    }
    
    //handle key utility harus berurutan mulai dari 1 sampai n 
    int expected = 1;
    for (const auto& entry : result) {
        if (entry.first != expected) {
            throw FileTidakValidException();
        }
        expected++;
    }

    //pengali tidak boleh negatif.
    for (const auto& entry : result) {
        if (entry.second < 0) {
            throw FileTidakValidException();
        }
    }

    return result;
}

ConfigLoader::TaxData ConfigLoader::loadTax(const vector<string>& lines) {
    for (const string& line : lines) {
        if (line.empty()) {
            continue;
        }

        istringstream iss(line);
        TaxData data;
        if (!(iss >> data.pphFlat >> data.pphPersen >> data.pbmFlat)) {
            continue;
        }
        return data;
    }
    //handle kalau kosong
    throw FileTidakValidException();
}

ConfigLoader::SpecialData ConfigLoader::loadSpecial(const vector<string>& lines) {
    for (const string& line : lines) {
        if (line.empty()) {
            continue;
        }

        istringstream iss(line);
        SpecialData data;
        if (!(iss >> data.gajiGo >> data.dendaPenjara)) {
            continue;
        }
        return data;
    }
    //handle kalau kosong
    throw FileTidakValidException();
}

ConfigLoader::MiscData ConfigLoader::loadMisc(const vector<string>& lines) {
    for (const string& line : lines) {
        if (line.empty()) {
            continue;
        }

        istringstream iss(line);
        MiscData data;
        if (!(iss >> data.maxTurn >> data.uangAwal)) {
            continue;
        }
        return data;
    }
    //handle kalau kosong
    throw FileTidakValidException();
}

map<int, AksiConfig> ConfigLoader::loadAksi(const vector<string>& lines){
    map<int, AksiConfig> aksiMap;
   
    for (const string& line : lines) {
        if (line.empty()) {
            continue;
        }

        istringstream iss(line);
        int id;
        string kode;
        string nama;
        string jenis;
        string warna;
        
        if (!(iss >> id >> kode >> nama >> jenis >> warna)) {
            continue;
        }
        AksiConfig petakAksi(id, kode, nama, jenis, warna);
        aksiMap[id] = petakAksi;

    }

    //handle kalau kosong
    if (aksiMap.empty()) {
        throw FileTidakValidException();
    }

    return aksiMap;
}