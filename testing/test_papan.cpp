#include <iostream>
#include "models/Papan.hpp"
#include "models/ConfigData.hpp"

int main(){
    std::cout << "=== TEST PAPAN ===" << std::endl;

    // NOTE: File ini tampaknya untuk eksperimen lama dan tidak masuk build utama.
    // Setelah refactor Papan (constructor injection), test ini perlu diupdate total agar sesuai struktur ConfigData sekarang.
    // Untuk sementara, biarkan sebagai placeholder agar tidak menyesatkan.
    std::cout << "Test papan placeholder (perlu disesuaikan dengan ConfigLoader + config/*.txt)\n";
    std::map<int, int> railroadMap = {
        {1, 25}, {2, 50}, {3, 100}, {4, 200}
    };
    std::map<int, int> utilityMap = {
        {1, 4}, {2, 10}
    };

    std::cout << "=== END TEST ===" << std::endl;

    return 0;
}