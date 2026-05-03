#include "../../include/views/OutputHandler.hpp"
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

std::string OutputHandler::getColorCode(const std::string& warna) {
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

std::string OutputHandler::resetColor() {
    return "\033[0m";
}

std::string OutputHandler::getBangunanSimbol(int level) {
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

const PropertiConfig* OutputHandler::findPropertiConfig(PetakProperti* properti, const ConfigData& config) {
    if (!properti) return nullptr;
    
    for (const auto& pair : config.getPropertiMap()) {
        if (pair.second.getKode() == properti->getKode()) {
            return &pair.second;
        }
    }
    return nullptr;
}

void OutputHandler::cetakAktaProperti(PetakProperti* properti, const ConfigData& config) {
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

void OutputHandler::cetakPapan(const Papan& papan, const std::vector<Pemain*>& daftarPemain, 
                          int currentTurn, int maxTurn) {
    if (papan.getTotalPetak() == 0) {
        std::cout << "Papan belum diinisialisasi.\n";
        return;
    }

    const int total = papan.getTotalPetak();
    const int cellWidth = 10;

    auto padRight = [](const std::string& s, size_t width) {
        if (s.size() >= width) return s.substr(0, width);
        return s + std::string(width - s.size(), ' ');
    };

    auto joinNumbers = [](const std::vector<int>& nums, const std::string& sep) {
        std::string out;
        for (size_t i = 0; i < nums.size(); ++i) {
            if (i > 0) out += sep;
            out += std::to_string(nums[i]);
        }
        return out;
    };

    auto shortColor = [](const std::string& warna) {
        if (warna == "COKLAT") return std::string("CK");
        if (warna == "BIRU_MUDA") return std::string("BM");
        if (warna == "MERAH_MUDA" || warna == "PINK") return std::string("PK");
        if (warna == "ABU_ABU") return std::string("AB");
        if (warna == "ORANGE") return std::string("OR");
        if (warna == "MERAH") return std::string("MR");
        if (warna == "KUNING") return std::string("KN");
        if (warna == "HIJAU") return std::string("HJ");
        if (warna == "BIRU_TUA") return std::string("BT");
        return std::string("DF");
    };

    auto getPetakByIndex = [&](int idx) -> Petak* {
        return const_cast<Papan&>(papan).getPetak(idx);
    };

    // Peta indeks pemain untuk label P1-P4.
    std::map<Pemain*, int> playerIndex;
    for (size_t i = 0; i < daftarPemain.size(); ++i) {
        if (daftarPemain[i]) playerIndex[daftarPemain[i]] = static_cast<int>(i) + 1;
    }

    int penIndex = -1;
    for (int i = 1; i <= total; ++i) {
        Petak* petak = getPetakByIndex(i);
        if (petak && petak->getKode() == "PEN") {
            penIndex = i;
            break;
        }
    }

    std::vector<std::vector<int>> tokens(total + 1);
    std::vector<int> jailIn;
    std::vector<int> jailVisit;
    for (size_t i = 0; i < daftarPemain.size(); ++i) {
        Pemain* p = daftarPemain[i];
        if (!p || p->getStatus() == StatusPemain::BANKRUPT) continue;
        int pos = p->getPosisi();
        if (pos < 1 || pos > total) pos = 1;
        int marker = static_cast<int>(i) + 1;
        if (pos == penIndex && penIndex != -1) {
            if (p->getStatus() == StatusPemain::JAILED) jailIn.push_back(marker);
            else jailVisit.push_back(marker);
        } else {
            tokens[pos].push_back(marker);
        }
    }

    auto formatCodeLine = [&](Petak* petak) {
        if (!petak) return std::string(cellWidth, ' ');
        std::string warna = "DEFAULT";
        std::string tag = "DF";
        if (auto prop = dynamic_cast<PetakProperti*>(petak)) {
            const PropertiConfig* cfg = prop->getConfigProperti();
            if (cfg) warna = cfg->getWarna();
            tag = shortColor(warna);
        }
        std::string label = "[" + tag + "] " + petak->getKode();
        std::string visible = padRight(label, cellWidth);
        if (dynamic_cast<PetakLahan*>(petak)) {
            return getColorCode(warna) + visible + resetColor();
        }
        return visible;
    };

    auto formatOwnerLine = [&](Petak* petak) {
        if (!petak) return std::string(cellWidth, ' ');
        auto prop = dynamic_cast<PetakProperti*>(petak);
        if (!prop) return std::string(cellWidth, ' ');

        std::string owner = "BANK";
        Pemain* pemilik = prop->getPemilik();
        if (pemilik) {
            auto it = playerIndex.find(pemilik);
            owner = (it != playerIndex.end()) ? ("P" + std::to_string(it->second)) : "P?";
        }
        if (prop->getStatus() == PetakProperti::StatusProperti::MORTGAGED) {
            owner += " M";
        }

        std::string building;
        if (auto street = dynamic_cast<PetakLahan*>(prop)) {
            if (street->punyaHotel()) building = "*";
            else if (street->getJumlahBangunan() > 0) building = std::string(street->getJumlahBangunan(), '^');
        }
        if (!building.empty()) owner += " " + building;
        return padRight(owner, cellWidth);
    };

    auto formatTokenLine = [&](int idx) {
        if (idx < 1 || idx > total) return std::string(cellWidth, ' ');
        std::string out;
        if (idx == penIndex && penIndex != -1) {
            std::string in = joinNumbers(jailIn, ",");
            std::string visit = joinNumbers(jailVisit, ",");
            if (!in.empty()) out += "I" + in;
            if (!visit.empty()) {
                if (!out.empty()) out += " ";
                out += "V" + visit;
            }
        } else {
            out = joinNumbers(tokens[idx], " ");
        }
        return padRight(out, cellWidth);
    };

    if (total < 4 || (total - 4) % 4 != 0) {
        std::cout << "\nPapan tidak berbentuk persegi standar. Menampilkan daftar petak:\n";
        for (int i = 1; i <= total; ++i) {
            Petak* petak = getPetakByIndex(i);
            if (!petak) continue;
            std::string info = petak->getKode() + " - " + petak->getNama();
            if (auto prop = dynamic_cast<PetakProperti*>(petak)) {
                std::string owner = formatOwnerLine(prop);
                info += " | " + owner;
            }
            std::string tok;
            if (i == penIndex && penIndex != -1) {
                std::string in = joinNumbers(jailIn, ",");
                std::string visit = joinNumbers(jailVisit, ",");
                if (!in.empty()) tok += "I" + in;
                if (!visit.empty()) {
                    if (!tok.empty()) tok += " ";
                    tok += "V" + visit;
                }
            } else {
                tok = joinNumbers(tokens[i], " ");
            }
            if (!tok.empty()) info += " | Bidak: " + tok;
            std::cout << std::setw(2) << i << ". " << info << "\n";
        }
        std::cout << "\nTURN " << currentTurn << " / " << maxTurn << "\n\n";
        return;
    }

    const int perSide = (total - 4) / 4;
    const int sideLen = perSide + 2;
    const int totalWidth = sideLen * cellWidth + (sideLen + 1);
    const int centerWidth = totalWidth - (cellWidth + 2) * 2;

    auto buildBorder = [&](int cells) {
        std::string line = "+";
        for (int i = 0; i < cells; ++i) line += std::string(cellWidth, '-') + "+";
        return line;
    };

    auto buildMiddleBorder = [&]() {
        return "+" + std::string(cellWidth, '-') + "+" + std::string(centerWidth, ' ') + "+" + std::string(cellWidth, '-') + "+";
    };

    std::vector<int> top(sideLen);
    std::vector<int> bottom(sideLen);
    std::vector<int> left(perSide);
    std::vector<int> right(perSide);

    for (int i = 0; i < sideLen; ++i) {
        top[i] = (2 * perSide + 3) + i;
        bottom[i] = (perSide + 2) - i;
    }
    for (int i = 0; i < perSide; ++i) {
        left[i] = (2 * perSide + 2) - i;
        right[i] = (3 * perSide + 5) + i;
    }

    std::string fullBorder = buildBorder(sideLen);
    std::string midBorder = buildMiddleBorder();

    std::cout << "\n" << fullBorder << "\n";

    std::string line;

    line = "|";
    for (int idx : top) line += formatCodeLine(getPetakByIndex(idx)) + "|";
    std::cout << line << "\n";

    line = "|";
    for (int idx : top) line += formatOwnerLine(getPetakByIndex(idx)) + "|";
    std::cout << line << "\n";

    line = "|";
    for (int idx : top) line += formatTokenLine(idx) + "|";
    std::cout << line << "\n";

    std::cout << (perSide > 0 ? midBorder : fullBorder) << "\n";

    for (int i = 0; i < perSide; ++i) {
        int leftIdx = left[i];
        int rightIdx = right[i];

        std::cout << "|" << formatCodeLine(getPetakByIndex(leftIdx)) << "|"
                  << std::string(centerWidth, ' ') << "|"
                  << formatCodeLine(getPetakByIndex(rightIdx)) << "|\n";

        std::cout << "|" << formatOwnerLine(getPetakByIndex(leftIdx)) << "|"
                  << std::string(centerWidth, ' ') << "|"
                  << formatOwnerLine(getPetakByIndex(rightIdx)) << "|\n";

        std::cout << "|" << formatTokenLine(leftIdx) << "|"
                  << std::string(centerWidth, ' ') << "|"
                  << formatTokenLine(rightIdx) << "|\n";

        std::cout << midBorder << "\n";
    }

    line = "|";
    for (int idx : bottom) line += formatCodeLine(getPetakByIndex(idx)) + "|";
    std::cout << line << "\n";

    line = "|";
    for (int idx : bottom) line += formatOwnerLine(getPetakByIndex(idx)) + "|";
    std::cout << line << "\n";

    line = "|";
    for (int idx : bottom) line += formatTokenLine(idx) + "|";
    std::cout << line << "\n";

    std::cout << fullBorder << "\n";

    std::cout << "\nTURN " << currentTurn << " / " << maxTurn << "\n";
    std::cout << "Legenda Kepemilikan & Status:\n";
    std::cout << "P1-P" << daftarPemain.size() << " : Properti milik pemain\n";
    std::cout << "M       : Properti digadai\n";
    std::cout << "^..*    : Rumah/Hotel pada petak lahan\n";
    std::cout << "I/V     : Tahanan (I) / Mampir (V) di PEN\n";
    std::cout << "1-4     : Bidak pemain\n";
    std::cout << "Kode warna: [CK]=Coklat [BM]=Biru Muda [PK]=Pink [OR]=Orange [MR]=Merah [KN]=Kuning [HJ]=Hijau [BT]=Biru Tua [AB]=Utilitas [DF]=Aksi\n\n";
}

void OutputHandler::cetakPropertiPemain(const Pemain* pemain, const ConfigData& config) {
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

void OutputHandler::tampilkanPropertiYangBisaDigadai(Pemain* pemain, const ConfigData& config) {
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

void OutputHandler::tampilkanPropertiYangBisaDitebus(Pemain* pemain, const ConfigData& config) {
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

void OutputHandler::tampilkanPropertiYangBisaDibangun(Pemain* pemain, ManagerProperti& manager, 
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




void OutputHandler::cetakPesan(const std::string& pesan) {
    std::cout << pesan << "\n";
}

void OutputHandler::cetakError(const std::string& errorMsg) {
    std::cout << "[ERROR] " << errorMsg << "\n";
}

void OutputHandler::cetakMendaratDiPetak(const std::string& namaPemain, const std::string& namaPetak) {
    std::cout << "\n" << namaPemain << " mendarat di petak " << namaPetak << "!\n";
}

void OutputHandler::cetakEfekKartu(const std::string& namaKartu, const std::string& pesan) {
    std::cout << "[" << namaKartu << "] " << pesan << "\n";
}

void OutputHandler::cetakTransaksi(const std::string& pembayar, int jumlah, const std::string& penerima) {
    if (penerima.empty() || penerima == "Bank") {
        std::cout << pembayar << " membayar M" << jumlah << " ke Bank.\n";
    } else {
        std::cout << pembayar << " membayar M" << jumlah << " ke " << penerima << ".\n";
    }
}

void OutputHandler::cetakAksi(const std::string& namaPemain, const std::string& aksi) {
    std::cout << namaPemain << " " << aksi << "\n";
}
