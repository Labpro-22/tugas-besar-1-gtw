<<<<<<< HEAD
#include "models/Managers/ManagerLelang.hpp"
#include "models/Petak/PetakProperti.hpp"
#include "models/Pemain.hpp"
#include "models/Managers/ManagerTransaksi.hpp"
#include "views/OutputHandler.hpp"
#include "views/InputHandler.hpp"
#include <algorithm>
#include <iostream>

void ManagerLelang::mulaiLelang(PetakProperti* properti, const std::vector<Pemain*>& daftarPemain, Pemain* pemainPemicu) {
    std::vector<Pemain*> partisipan;
    
    // Temukan indeks pemain pemicu
    int startIndex = -1;
    for (size_t i = 0; i < daftarPemain.size(); ++i) {
        if (daftarPemain[i] == pemainPemicu) {
            startIndex = i;
            break;
        }
    }

    // Masukkan pemain mulai dari pemain SETELAH pemicu
    if (startIndex != -1) {
        for (size_t i = 1; i <= daftarPemain.size(); ++i) {
            size_t idx = (startIndex + i) % daftarPemain.size();
            Pemain* p = daftarPemain[idx];
            // Hanya pemain aktif (termasuk di penjara) yang bisa ikut lelang
            if (p->getStatus() != StatusPemain::BANKRUPT) {
                partisipan.push_back(p);
            }
        }
    } else {
        // Jika karena suatu hal tidak ketemu, masukkan semua yang aktif
        for (Pemain* p : daftarPemain) {
            if (p->getStatus() != StatusPemain::BANKRUPT) {
                partisipan.push_back(p);
            }
        }
    }

    if (partisipan.empty()) {
        OutputHandler::cetakError("Tidak ada pemain yang bisa mengikuti lelang.");
        return;
    }

    OutputHandler::cetakPesan("=== LELANG DIBUKA UNTUK PROPERTI " + properti->getNama() + " (" + properti->getKode() + ") ===");
    OutputHandler::cetakPesan("Harga awal (nilai properti): M" + std::to_string(properti->getConfigProperti()->getHargaLahan()));

    const int n = static_cast<int>(partisipan.size());
    int bidTertinggi = 0;
    Pemain* pemenangSementara = nullptr;
    int passBeruntun = 0;
    bool pernahAdaBid = false;
    int indeksSekarang = 0;

    while (true) {
        Pemain* p = partisipan[indeksSekarang];
        OutputHandler::cetakPesan("\nGiliran " + p->getUsername() + " (Bid tertinggi saat ini: M" + std::to_string(bidTertinggi) + ")");

        int bid = InputHandler::promptAngka("Masukkan tawaran (0 untuk PASS): M", 0, p->getSaldo());

        if (bid == 0) {
            // Sesuai spek: minimal harus ada satu bidder.
            // Jika sudah (N-1) PASS beruntun dan belum ada bid, pemain ini tidak boleh PASS lagi.
            if (!pernahAdaBid && passBeruntun == n - 1) {
                OutputHandler::cetakError("Minimal harus ada satu BID. Kamu wajib memasang tawaran.");
                continue;
            }

            OutputHandler::cetakPesan(p->getUsername() + " memilih PASS.");
            passBeruntun++;

            if (pernahAdaBid && passBeruntun >= n - 1) {
                break;
            }
        } else if (bid <= bidTertinggi) {
            OutputHandler::cetakError("Tawaran harus lebih tinggi dari M" + std::to_string(bidTertinggi) + "!");
            continue;
        } else {
            bidTertinggi = bid;
            pemenangSementara = p;
            pernahAdaBid = true;
            passBeruntun = 0;
            OutputHandler::cetakPesan(p->getUsername() + " menawar M" + std::to_string(bidTertinggi));
        }

        indeksSekarang = (indeksSekarang + 1) % n;
    }

    if (pemenangSementara == nullptr) {
        // Harusnya tidak terjadi karena ada aturan minimal satu bid, tapi aman-kan fallback.
        OutputHandler::cetakPesan("Lelang berakhir tanpa pemenang. Properti kembali ke Bank.");
        properti->setPemilik(nullptr);
        properti->setStatus(PetakProperti::StatusProperti::BANK);
        return;
    }

    OutputHandler::cetakPesan("\n=== LELANG SELESAI ===");
    OutputHandler::cetakPesan("Pemenang: " + pemenangSementara->getUsername());
    OutputHandler::cetakPesan("Harga akhir: M" + std::to_string(bidTertinggi));

    try {
        managerTransaksi->transferMoney(pemenangSementara, nullptr, bidTertinggi);
        properti->setPemilik(pemenangSementara);
        properti->setStatus(PetakProperti::StatusProperti::OWNED);
        pemenangSementara->tambahAset(properti);
    } catch (...) {
        OutputHandler::cetakError("Terjadi kesalahan sistem saat memproses pembayaran lelang.");
    }
}
=======
#include "models/Managers/ManagerLelang.hpp"
#include "models/Petak/PetakProperti.hpp"
#include "models/Pemain.hpp"
#include "models/Managers/ManagerTransaksi.hpp"
#include "views/OutputHandler.hpp"
#include "views/InputHandler.hpp"
#include <algorithm>
#include <iostream>

void ManagerLelang::mulaiLelang(PetakProperti* properti, const std::vector<Pemain*>& daftarPemain, Pemain* pemainPemicu) {
    std::vector<Pemain*> partisipan;
    
    // Temukan indeks pemain pemicu
    int startIndex = -1;
    for (size_t i = 0; i < daftarPemain.size(); ++i) {
        if (daftarPemain[i] == pemainPemicu) {
            startIndex = i;
            break;
        }
    }

    // Masukkan pemain mulai dari pemain SETELAH pemicu
    if (startIndex != -1) {
        for (size_t i = 1; i <= daftarPemain.size(); ++i) {
            size_t idx = (startIndex + i) % daftarPemain.size();
            Pemain* p = daftarPemain[idx];
            // Hanya pemain aktif (termasuk di penjara) yang bisa ikut lelang
            if (p->getStatus() != StatusPemain::BANKRUPT) {
                partisipan.push_back(p);
            }
        }
    } else {
        // Jika karena suatu hal tidak ketemu, masukkan semua yang aktif
        for (Pemain* p : daftarPemain) {
            if (p->getStatus() != StatusPemain::BANKRUPT) {
                partisipan.push_back(p);
            }
        }
    }

    if (partisipan.empty()) {
        OutputHandler::cetakError("Tidak ada pemain yang bisa mengikuti lelang.");
        return;
    }

    OutputHandler::cetakPesan("=== LELANG DIBUKA UNTUK PROPERTI " + properti->getNama() + " (" + properti->getKode() + ") ===");
    OutputHandler::cetakPesan("Harga awal (nilai properti): M" + std::to_string(properti->getConfigProperti()->getHargaLahan()));

    int bidTertinggi = 0;
    Pemain* pemenangSementera = nullptr;
    int indeksSekarang = 0;

    while (partisipan.size() > 1) {
        Pemain* p = partisipan[indeksSekarang];
        OutputHandler::cetakPesan("\nGiliran " + p->getUsername() + " (Bid tertinggi saat ini: M" + std::to_string(bidTertinggi) + ")");
        
        int bid = InputHandler::promptAngka("Masukkan tawaran (Ketik 0 untuk PASS): M", 0, p->getSaldo());

        if (bid == 0) {
            OutputHandler::cetakPesan(p->getUsername() + " mengundurkan diri dari lelang.");
            partisipan.erase(partisipan.begin() + indeksSekarang);
            // Jangan tambah indeksSekarang karena elemen bergeser
            if (indeksSekarang >= static_cast<int>(partisipan.size())) {
                indeksSekarang = 0;
            }
        } else if (bid <= bidTertinggi) {
            OutputHandler::cetakError("Tawaran harus lebih tinggi dari M" + std::to_string(bidTertinggi) + "!");
            // Ulangi giliran pemain ini
        } else {
            bidTertinggi = bid;
            pemenangSementera = p;
            OutputHandler::cetakPesan(p->getUsername() + " menawar M" + std::to_string(bidTertinggi));
            indeksSekarang = (indeksSekarang + 1) % partisipan.size();
        }
    }

    // Hasil akhir
    if (partisipan.empty() || pemenangSementera == nullptr) {
        OutputHandler::cetakPesan("Semua pemain PASS. Properti " + properti->getNama() + " tidak terjual dan kembali ke Bank.");
        // Kembalikan ke bank
        properti->setPemilik(nullptr);
        properti->setStatus(PetakProperti::StatusProperti::BANK);
    } else {
        Pemain* pemenangFinal = partisipan[0];
        OutputHandler::cetakPesan("\n=== LELANG SELESAI ===");
        OutputHandler::cetakPesan(pemenangFinal->getUsername() + " memenangkan properti " + properti->getNama() + " seharga M" + std::to_string(bidTertinggi) + "!");
        
        try {
            // Pemain membayar ke bank (penerima null)
            managerTransaksi->transferMoney(pemenangFinal, nullptr, bidTertinggi);
            
            // Set kepemilikan
            properti->setPemilik(pemenangFinal);
            properti->setStatus(PetakProperti::StatusProperti::OWNED);
            pemenangFinal->tambahAset(properti);
        } catch (...) {
            OutputHandler::cetakError("Terjadi kesalahan sistem saat memproses pembayaran lelang.");
        }
    }
}
>>>>>>> 4c863580e8432dbd781567a56c63fe50b90b3f39
