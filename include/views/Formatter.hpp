#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <random>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <map>
#include "../models/ConfigData.hpp"

// Forward declarations
class Pemain;
class Petak;
class Properti;
class PropertiStreet;
class ConfigData;
class ManagerProperti;

class Formatter {
public :
    // Menampilkan akta kepemilikan dari suatu properti
    static void cetakAktaProperti(Properti* properti, const ConfigData& config);
    
    // Menampilkan papan permainan dengan status pemain
    static void cetakPapan(const std::vector<Petak*>& papan, const std::vector<Pemain*>& daftarPemain, 
                          int currentTurn, int maxTurn);
    
    // Menampilkan properti milik pemain yang sedang bermain
    static void cetakPropertiPemain(const Pemain* pemain, const ConfigData& config);
    
    // Menampilkan properti yang bisa digadai
    static void tampilkanPropertiYangBisaDigadai(Pemain* pemain, const ConfigData& config);
    
    // Menampilkan properti yang sedang digadai (bisa ditebus)
    static void tampilkanPropertiYangBisaDitebus(Pemain* pemain, const ConfigData& config);
    
    // Menampilkan properti yang bisa dibangun
    static void tampilkanPropertiYangBisaDibangun(Pemain* pemain, ManagerProperti& manager, 
                                                  const ConfigData& config);
    
private:
    // Helper function untuk mendapatkan warna ANSI untuk display
    static std::string getColorCode(const std::string& warna);
    static std::string resetColor();
    
    // Helper function untuk mendapatkan simbol bangunan
    static std::string getBangunanSimbol(int level);
    
    // Helper untuk mencari PropertiConfig dari Properti
    static const PropertiConfig* findPropertiConfig(Properti* properti, const ConfigData& config);
};
