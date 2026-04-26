#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "../models/ConfigData.hpp"
#include "../models/Pemain.hpp"
#include "../models/Petak/PetakProperti.hpp"
#include "../models/Petak/PetakLahan.hpp"
#include "../views/OutputHandler.hpp"
using namespace std;

class Kebangkrutan {
public :
    static void declareBangkrut (Pemain *p);

    static void tampilkanBangkrut (Pemain *p);

};

class Likuidasi {
protected:
    static const ConfigData* config;

public :
    static bool bisaBayarDenganLikuidasi (Pemain *p, int harga); 
    static bool bisaBayarLangsung (Pemain* p, int harga);

    static int totalLikuidasiSeluruhAsetGadai (Pemain *p); // likuidasinya gadai semua
    static int totalLikuidasiSeluruhAsetJual (Pemain *p); // likuidasinya jual semua

    static void tampilkanPanelLikuidasi (Pemain *p, int harga);

};

class SelesaiGame {
public :
    static Pemain* getPemenang (vector<Pemain*> listPemain);
    static bool sisaSatuOrang (vector<Pemain*> listPemain); // sisa satu orang yang belom bangkrut

    static void tampilPanelSelesaiKarenaMaxTurn (int maxTurn);
    static void tampilPanelSelesaiKarenaBangkrut ();
    static void tampilkanPemenang (Pemain *p);

};