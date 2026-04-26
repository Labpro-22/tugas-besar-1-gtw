#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "../models/ConfigData.hpp"
#include "../models/Pemain.hpp"
using namespace std;

class Kebangkrutan {
public :
    static bool bisaBayar (Pemain *p, int harga);

    static void tampilPanelBayar (); // kayaknya harusnya ada parameternya

    static void tampilkanBangkrut (Pemain *p);

};


class SelesaiGame {
public :
    static void tampilPanelSelesaiKarenaMaxTurn ();
    static void tampilPanelSelesaiKarenaBangkrut ();
    static void tampilkanPemenang (Pemain p);

};