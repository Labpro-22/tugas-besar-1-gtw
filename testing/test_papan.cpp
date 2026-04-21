#include <iostream>
#include "models/Papan.hpp"
#include "models/Properti/ManagerProperti.hpp"
#include "models/ConfigData.hpp"

int main(){
    std::cout << "=== TEST PAPAN ===" << std::endl;

    // 🔹 Dummy ConfigData (minimal)
    std::map<std::string, PropertiConfig> propertiMap;
    std::map<int, int> railroadMap = {
        {1, 25}, {2, 50}, {3, 100}, {4, 200}
    };
    std::map<int, int> utilityMap = {
        {1, 4}, {2, 10}
    };

    ConfigData config(
        100, 10, 200, 200, 50,
        100, 1500,
        propertiMap,
        railroadMap,
        utilityMap
    );

    // 🔹 ManagerProperti
    ManagerProperti manager(config);

    // 🔹 Papan
    Papan papan(manager);

    // =============================
    // TEST 1: Jumlah petak
    // =============================
    std::cout << "Total petak: " << papan.getTotalPetak() << std::endl;

    if(papan.getTotalPetak() == 40){
        std::cout << "✔ PASS: jumlah petak benar\n";
    } else {
        std::cout << "✖ FAIL: jumlah petak salah\n";
    }

    // =============================
    // TEST 2: getPetak by indeks
    // =============================
    Petak* p1 = papan.getPetak(1);

    if(p1){
        std::cout << "Petak 1: " << p1->getDisplayInfo() << std::endl;
    } else {
        std::cout << "✖ FAIL: getPetak(1) null\n";
    }

    // =============================
    // TEST 3: getPetak by kode
    // =============================
    Petak* pGarut = papan.getPetak("GRT");

    if(pGarut){
        std::cout << "GRT ditemukan: " << pGarut->getDisplayInfo() << std::endl;
    } else {
        std::cout << "✖ FAIL: GRT tidak ditemukan\n";
    }

    // =============================
    // TEST 4: findNearestByKode
    // =============================
    Petak* nearestStation = papan.findNearestByKode("GBR", 1);

    if(nearestStation){
        std::cout << "Nearest GBR dari 1: "
                  << nearestStation->getDisplayInfo() << std::endl;
    } else {
        std::cout << "✖ FAIL: findNearest gagal\n";
    }

    // =============================
    // TEST 5: invalid indeks
    // =============================
    if(papan.getPetak(100) == nullptr){
        std::cout << "✔ PASS: invalid indeks handled\n";
    } else {
        std::cout << "✖ FAIL: invalid indeks tidak null\n";
    }

    std::cout << "=== END TEST ===" << std::endl;

    return 0;
}