#include "views/InputHandler.hpp"

int InputHandler::promptAngka(const std::string& pesan, int min, int max) {
    int pilihan = 0;
    while (true) {
        std::cout << pesan;
        std::cin >> pilihan;
        if (std::cin.fail() || pilihan < min || pilihan > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Masukan tidak valid. Harap masukkan angka antara " << min << " dan " << max << ".\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return pilihan;
        }
    }
}

std::string InputHandler::promptString(const std::string& pesan) {
    std::string input;
    std::cout << pesan;
    std::cin >> input;
    return input;
}

int InputHandler::mintaPilihanPajak(int pphFlat, int pphPersen) {
    std::cout << "\nKamu mendarat di Petak Pajak Penghasilan (PPH)!\n";
    std::cout << "Pilih opsi pembayaran pajak:\n";
    std::cout << "1. Bayar flat M" << pphFlat << "\n";
    std::cout << "2. Bayar " << pphPersen << "% dari total kekayaan\n";
    std::cout << "(Pilih sebelum menghitung kekayaan!)\n";
    
    return promptAngka("Pilihan (1/2): ", 1, 2);
}
