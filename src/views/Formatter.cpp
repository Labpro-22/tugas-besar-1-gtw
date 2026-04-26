#include "../../include/views/Formatter.hpp"
#include "../../include/models/Pemain.hpp"
#include "../../include/models/Petak/Petak.hpp"
#include "../../include/models/Petak/PetakProperti.hpp"
#include "../../include/models/Petak/PetakProperti.hpp"
#include "../../include/models/Petak/PetakLahan.hpp"
#include "../../include/models/Petak/PetakStasiun.hpp"
#include "../../include/models/Petak/PetakUtilitas.hpp"
#include "models/Managers/ManagerProperti.hpp"
#include "../../include/models/ConfigData.hpp"
#include "../../include/models/Papan.hpp"

std::string Formatter::getColorCode(const std::string& warna) {
    if (warna == "COKLAT") return "\033[38;5;94m";          // Brown
    if (warna == "BIRU_MUDA") return "\033[38;5;51m";       // Cyan
    if (warna == "MERAH_MUDA" || warna == "PINK") return "\033[38;5;205m";  // Magenta
    if (warna == "ABU_ABU") return "\033[38;5;8m";           // Gray
    if (warna == "ORANGE") return "\033[38;5;208m";          // Orange
    if (warna == "MERAH") return "\033[31m";                 // Red
    if (warna == "KUNING") return "\033[33m";                // Yellow
    if (warna == "HIJAU") return "\033[32m";                 // Green
    if (warna == "BIRU_TUA") return "\033[34m";              // Blue
    return "\033[0m";  // Default
}

std::string Formatter::resetColor() {
    return "\033[0m";
}

std::string Formatter::getBangunanSimbol(int level) {
    switch(level) {
        case 0: return "  ";
        case 1: return "^";
        case 2: return "^^";
        case 3: return "^^^";
        case 4: return "^^^^";
        case 5: return "*";  // Hotel
        default: return "  ";
    }
}

const PropertiConfig* Formatter::findPropertiConfig(PetakProperti* properti, const ConfigData& config) {
    if (!properti) return nullptr;
    
    for (const auto& pair : config.getPropertiMap()) {
        if (pair.second.getKode() == properti->getKode()) {
            return &pair.second;
        }
    }
    return nullptr;
}

void Formatter::cetakAktaProperti(PetakProperti* properti, const ConfigData& config) {
    if (!properti) {
        std::cout << "PetakProperti tidak valid.\n";
        return;
    }
    
    const PropertiConfig* propConfig = findPropertiConfig(properti, config);
    if (!propConfig) {
        std::cout << "Petak \"" << properti->getKode() << "\" tidak ditemukan atau bukan properti.\n";
        return;
    }
    
    std::string colorCode = getColorCode(propConfig->getWarna());
    std::string reset = resetColor();
    
    std::cout << "\n" << "+================================+\n";
    std::cout << "| AKTA KEPEMILIKAN |\n";
    std::cout << "| " << colorCode << "[" << propConfig->getWarna() << "] " 
              << propConfig->getNama() << " (" << propConfig->getKode() << ")" 
              << reset << " |\n";
    std::cout << "+================================+\n";
    
    if (propConfig->getJenis() == "STREET") {
        std::cout << "| Harga Beli : M" << propConfig->getHargaLahan() << " |\n";
        std::cout << "| Nilai Gadai : M" << propConfig->getNilaiGadai() << " |\n";
        std::cout << "+--------------------------------+\n";
        
        // Tampilkan tabel sewa untuk setiap level
        const auto& sewaTabel = propConfig->getHargaSewa();
        std::cout << "| Sewa (unimproved) : M" << sewaTabel[0] << " |\n";
        for (size_t i = 1; i < sewaTabel.size() - 1; i++) {
            std::cout << "| Sewa (" << i << (i == 1 ? " rumah) " : " rumah) ")
                      << std::string(16 - std::to_string(sewaTabel[i]).length(), ' ')
                      << ": M" << sewaTabel[i] << " |\n";
        }
        std::cout << "| Sewa (hotel) : M" << sewaTabel.back() << " |\n";
        
        std::cout << "+--------------------------------+\n";
        std::cout << "| Harga Rumah : M" << propConfig->getHargaBangunRumah() << " |\n";
        std::cout << "| Harga Hotel : M" << propConfig->getHargaBangunHotel() << " |\n";
    } else if (propConfig->getJenis() == "RAILROAD") {
        std::cout << "| Harga Perolehan : Otomatis |\n";
        std::cout << "| Nilai Gadai : M" << propConfig->getNilaiGadai() << " |\n";
        std::cout << "+--------------------------------+\n";
        std::cout << "| Sewa bergantung jumlah Railroad |\n";
    } else if (propConfig->getJenis() == "UTILITY") {
        std::cout << "| Harga Perolehan : Otomatis |\n";
        std::cout << "| Nilai Gadai : M" << propConfig->getNilaiGadai() << " |\n";
        std::cout << "+--------------------------------+\n";
        std::cout << "| Sewa : (Total Dadu) x Faktor |\n";
    }
    
    std::cout << "+================================+\n";
    
    // Tampilkan status
    std::string statusStr = "BANK";
    if (properti->getStatus() == PetakProperti::StatusProperti::OWNED) {
        statusStr = "OWNED (" + properti->getPemilik()->getUsername() + ")";
    } else if (properti->getStatus() == PetakProperti::StatusProperti::MORTGAGED) {
        statusStr = "MORTGAGED";
    }
    
    std::cout << "| Status : " << statusStr << " |\n";
    std::cout << "+================================+\n\n";
}

void Formatter::cetakPapan(const Papan& papan, const std::vector<Pemain*>& daftarPemain, 
                          int currentTurn, int maxTurn) {
    if (papan.getTotalPetak() == 0) {
        std::cout << "Papan belum diinisialisasi.\n";
        return;
    }
    
    // Tampilkan header papan (simplified version)
    std::cout << "\n" << "+----------+----------+----------+----------+\n";
    std::cout << "|" << std::setw(40) << std::setfill(' ') << "NIMONSPOLI" << "|\n";
    std::cout << "+----------+----------+----------+----------+\n";
    std::cout << "\n";
    
    // Informasi giliran
    std::cout << "TURN " << currentTurn << " / " << maxTurn << "\n\n";
    
    // Tampilkan daftar pemain dan posisinya
    for (size_t i = 0; i < daftarPemain.size(); i++) {
        const Pemain* p = daftarPemain[i];
        Petak* petak = const_cast<Papan&>(papan).getPetak(p->getPosisi());
        if (petak) {
            std::cout << "P" << (i+1) << ": " << p->getUsername() 
                      << " - Saldo: M" << p->getSaldo() 
                      << " - Posisi: " << petak->getKode() << "\n";
        }
    }
    
    std::cout << "\n";
}

void Formatter::cetakPropertiPemain(const Pemain* pemain, const ConfigData& config) {
    if (!pemain) {
        std::cout << "Pemain tidak valid.\n";
        return;
    }
    
    const auto& aset = pemain->getAsetPemain();
    
    if (aset.empty()) {
        std::cout << "\nKamu belum memiliki properti apapun.\n\n";
        return;
    }
    
    std::cout << "\n=== PetakProperti Milik: " << pemain->getUsername() << " ===\n";
    
    // Kelompokkan properti berdasarkan warna/jenis
    std::map<std::string, std::vector<PetakProperti*>> grupProperti;
    int totalKekayaan = 0;
    
    for (PetakProperti* prop : aset) {
        if (prop) {
            try {
                // Cari warna properti dari config
                for (const auto& pair : config.getPropertiMap()) {
                    if (pair.second.getKode() == prop->getKode()) {
                        grupProperti[pair.second.getWarna()].push_back(prop);
                        totalKekayaan += pair.second.getHargaLahan();
                        break;
                    }
                }
            } catch (...) {
                // Skip jika tidak ditemukan
            }
        }
    }
    
    // Tampilkan per grup warna
    for (const auto& grup : grupProperti) {
        std::string colorCode = getColorCode(grup.first);
        std::string reset = resetColor();
        
        std::cout << "[" << colorCode << grup.first << reset << "]\n";
        
        for (PetakProperti* prop : grup.second) {
            std::string statusStr = (prop->getStatus() == PetakProperti::StatusProperti::MORTGAGED) 
                                    ? " MORTGAGED [M]" : " OWNED";
            
            // Tampilkan info properti
            std::cout << "- " << prop->getNama() << " (" << prop->getKode() << ")";
            
            // Jika ada bangunan, tampilkan
            PetakLahan* street = dynamic_cast<PetakLahan*>(prop);
            if (street) {
                if (street->punyaHotel()) {
                    std::cout << " Hotel";
                } else if (street->getJumlahBangunan() > 0) {
                    std::cout << " " << street->getJumlahBangunan() << " rumah";
                }
            }
            
            std::cout << statusStr << "\n";
        }
    }
    
    std::cout << "\nTotal kekayaan properti: M" << totalKekayaan << "\n\n";
}

void Formatter::tampilkanPropertiYangBisaDigadai(Pemain* pemain, const ConfigData& config) {
    if (!pemain) {
        std::cout << "Pemain tidak valid.\n";
        return;
    }
    
    std::vector<PetakProperti*> bisaDigadai;
    std::vector<PetakProperti*> adaBangunan;
    const auto& aset = pemain->getAsetPemain();
    
    // Filter properti yang bisa digadai (status OWNED)
    for (PetakProperti* prop : aset) {
        if (prop && prop->getStatus() == PetakProperti::StatusProperti::OWNED) {
            // Cek jika tidak ada bangunan (untuk street)
            PetakLahan* street = dynamic_cast<PetakLahan*>(prop);
            if (!street || street->getJumlahBangunan() == 0) {
                bisaDigadai.push_back(prop);
            } else {
                // Jika ada bangunan, masukkan dalam list terpisah
                adaBangunan.push_back(prop);
            }
        }
    }
    
    if (bisaDigadai.empty() && adaBangunan.empty()) {
        std::cout << "Tidak ada properti yang dapat digadaikan saat ini.\n";
        return;
    }
    
    std::cout << "\n=== PetakProperti yang Dapat Digadaikan ===\n";
    
    int index = 1;
    
    // Tampilkan properti tanpa bangunan
    for (PetakProperti* prop : bisaDigadai) {
        const PropertiConfig* propConfig = findPropertiConfig(prop, config);
        if (propConfig) {
            std::cout << index << ". " << prop->getNama() << " (" << prop->getKode() << ")"
                      << " [" << propConfig->getWarna() << "] Nilai Gadai: M" << propConfig->getNilaiGadai() << "\n";
            index++;
        }
    }
    
    // Tampilkan properti dengan bangunan dengan catatan
    if (!adaBangunan.empty()) {
        std::cout << "\n(PetakProperti dengan bangunan - harus jual bangunan dulu):\n";
        for (PetakProperti* prop : adaBangunan) {
            const PropertiConfig* propConfig = findPropertiConfig(prop, config);
            if (propConfig) {
                PetakLahan* street = dynamic_cast<PetakLahan*>(prop);
                std::cout << index << ". " << prop->getNama() << " (" << prop->getKode() << ")"
                          << " [" << propConfig->getWarna() << "] - " << street->getJumlahBangunan() 
                          << " rumah\n";
                index++;
            }
        }
    }
    
    std::cout << "\n";
}

void Formatter::tampilkanPropertiYangBisaDitebus(Pemain* pemain, const ConfigData& config) {
    if (!pemain) {
        std::cout << "Pemain tidak valid.\n";
        return;
    }
    
    std::vector<PetakProperti*> sedangDigadai;
    const auto& aset = pemain->getAsetPemain();
    
    // Filter properti yang sedang digadai (status MORTGAGED)
    for (PetakProperti* prop : aset) {
        if (prop && prop->getStatus() == PetakProperti::StatusProperti::MORTGAGED) {
            sedangDigadai.push_back(prop);
        }
    }
    
    if (sedangDigadai.empty()) {
        std::cout << "Tidak ada properti yang sedang digadaikan.\n";
        return;
    }
    
    std::cout << "\n=== PetakProperti yang Sedang Digadaikan ===\n";
    for (size_t i = 0; i < sedangDigadai.size(); i++) {
        PetakProperti* prop = sedangDigadai[i];
        
        // Cari info dari config
        std::string warna;
        int hargaBeli = 0;
        for (const auto& pair : config.getPropertiMap()) {
            if (pair.second.getKode() == prop->getKode()) {
                warna = pair.second.getWarna();
                hargaBeli = pair.second.getHargaLahan();
                break;
            }
        }
        
        std::cout << (i+1) << ". " << prop->getNama() << " (" << prop->getKode() << ")"
                  << " [" << warna << "] [M] Harga Tebus: M" << hargaBeli << "\n";
    }
    std::cout << "Uang kamu saat ini: M" << pemain->getSaldo() << "\n\n";
}

void Formatter::tampilkanPropertiYangBisaDibangun(Pemain* pemain, ManagerProperti& manager, 
                                                  const ConfigData& config) {
    if (!pemain) {
        std::cout << "Pemain tidak valid.\n";
        return;
    }
    
    // Kelompokkan properti street berdasarkan color group
    std::map<std::string, std::vector<PetakLahan*>> grupBisaDibangun;
    
    const auto& aset = pemain->getAsetPemain();
    for (PetakProperti* prop : aset) {
        if (prop && prop->getStatus() == PetakProperti::StatusProperti::OWNED) {
            PetakLahan* street = dynamic_cast<PetakLahan*>(prop);
            if (street) {
                // Cek apakah punya monopoli
                if (manager.isMonopoly(pemain, street->getWarnaString())) {
                    // Ambil warna dari config
                    for (const auto& pair : config.getPropertiMap()) {
                        if (pair.second.getKode() == street->getKode()) {
                            grupBisaDibangun[pair.second.getWarna()].push_back(street);
                            break;
                        }
                    }
                }
            }
        }
    }
    
    if (grupBisaDibangun.empty()) {
        std::cout << "\nTidak ada color group yang memenuhi syarat untuk dibangun.\n";
        std::cout << "Kamu harus memiliki seluruh petak dalam satu color group terlebih dahulu.\n\n";
        return;
    }
    
    std::cout << "\n=== Color Group yang Memenuhi Syarat ===\n";
    int groupNum = 1;
    
    for (const auto& grup : grupBisaDibangun) {
        std::string colorCode = getColorCode(grup.first);
        std::string reset = resetColor();
        
        std::cout << groupNum << ". [" << colorCode << grup.first << reset << "]\n";
        
        for (PetakLahan* street : grup.second) {
            // Cari info dari config
            int hargaBangun = 0;
            for (const auto& pair : config.getPropertiMap()) {
                if (pair.second.getKode() == street->getKode()) {
                    hargaBangun = pair.second.getHargaBangunRumah();
                    break;
                }
            }
            
            int jumlahBangunan = street->getJumlahBangunan();
            std::string buildingStr;
            
            if (street->punyaHotel()) {
                buildingStr = "Hotel";
            } else if (jumlahBangunan > 0) {
                buildingStr = std::to_string(jumlahBangunan) + " rumah";
            } else {
                buildingStr = "0 rumah";
            }
            
            std::cout << "- " << street->getNama() << " (" << street->getKode() << ") : " 
                      << buildingStr << " (Harga rumah: M" << hargaBangun << ")\n";
        }
        
        groupNum++;
    }
    
    std::cout << "Uang kamu saat ini : M" << pemain->getSaldo() << "\n\n";
}
