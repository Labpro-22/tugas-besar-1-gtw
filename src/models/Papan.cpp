#include "models/Papan.hpp"

Papan::Papan(ManagerProperti& manager){
// Membuat 40 petak papan, diambil dari ManagerProperti
    std::vector<PetakInit> initData = {
        {1, "GO", "Petak Mulai", "SPESIAL"},
        {2, "GRT", "Garut", "LAHAN"},
        {3, "DNU", "Dana Umum", "KARTU"},
        {4, "TSK", "Tasikmalaya", "LAHAN"},
        {5, "PPH", "Pajak Penghasilan", "PAJAK"},
        {6, "GBR", "Stasiun Gambir", "STASIUN"},
        {7, "BGR", "Bogor", "LAHAN"},
        {8, "FES", "Festival", "FESTIVAL"},
        {9, "DPK", "Depok", "LAHAN"},
        {10, "BKS", "Bekasi", "LAHAN"},
        {11, "PEN", "Penjara", "SPESIAL"},
        {12, "MGL", "Magelang", "LAHAN"},
        {13, "PLN", "PLN", "UTILITAS"},
        {14, "SOL", "Solo", "LAHAN"},
        {15, "YOG", "Yogyakarta", "LAHAN"},
        {16, "STB", "Stasiun Bandung", "STASIUN"},
        {17, "MAL", "Malang", "LAHAN"},
        {18, "DNU", "Dana Umum", "KARTU"},
        {19, "SMG", "Semarang", "LAHAN"},
        {20, "SBY", "Surabaya", "LAHAN"},
        {21, "BBP", "Bebas Parkir", "SPESIAL"},
        {22, "MKS", "Makasar", "LAHAN"},
        {23, "KSP", "Kesempatan", "KARTU"},
        {24, "BLP", "Balikpapan", "LAHAN"},
        {25, "MND", "Manado", "LAHAN"},
        {26, "TUG", "Stasiun Tugu", "STASIUN"},
        {27, "PLB", "Palembang", "LAHAN"},
        {28, "PKB", "Pekanbaru", "LAHAN"},
        {29, "PAM", "PAM", "UTILITAS"},
        {30, "MED", "Medan", "LAHAN"},
        {31, "PPJ", "Petak Pergi ke Penjara", "SPESIAL"},
        {32, "BDG", "Bandung", "LAHAN"},
        {33, "DEN", "Denpasar", "LAHAN"},
        {34, "FES", "Festival", "FESTIVAL"},
        {35, "MTR", "Mataram", "LAHAN"},
        {36, "GUB", "Stasiun Gubeg", "STASIUN"},
        {37, "KSP", "Kesempatan", "KARTU"},
        {38, "JKT", "Jakarta", "LAHAN"},
        {39, "PMB", "Pajak Barang Mewah", "PAJAK"},
        {40, "IKN", "Ibu Kota Nusantara", "LAHAN"}
    };

    for(const auto& data : initData){
        // PROPERTY (Street, Railroad, Utility)
        if(data.tipe == "LAHAN" || data.tipe == "STASIUN" || data.tipe == "UTILITAS"){
            Properti* p = manager.getProperti(data.kode);

            daftarPetak.push_back(
                new PetakProperti(
                    data.indeks,
                    data.kode,
                    data.nama,
                    p
                )
            );
        }

        // AKSI
        else if(data.tipe == "KARTU"){
            daftarPetak.push_back(
                new PetakKartu(
                    data.indeks,
                    data.kode,
                    data.nama
                )
            );
        }
    
        else if(data.tipe == "PAJAK"){
            daftarPetak.push_back(
                new PetakPajak(
                    data.indeks,
                    data.kode,
                    data.nama
                )
            );
        }

        else if(data.tipe == "FESTIVAL"){
            daftarPetak.push_back(
                new PetakFestival(
                    data.indeks,
                    data.kode,
                    data.nama
                )
            );
        }

        // SPESIAL 
        else if(data.tipe == "SPESIAL"){
            if(data.kode == "GO"){
                daftarPetak.push_back(
                    new PetakMulai(
                        data.indeks,
                        data.kode,
                        data.nama
                    )
                );
            }
            else if(data.kode == "PEN"){
                daftarPetak.push_back(
                    new PetakPenjara(
                        data.indeks,
                        data.kode,
                        data.nama
                    )
                );
            }
            else if(data.kode == "PPJ"){
                daftarPetak.push_back(
                    new PetakPergiKePenjara(
                        data.indeks,
                        data.kode,
                        data.nama
                    )
                );
            }
            else if(data.kode == "BBP"){
                daftarPetak.push_back(
                    new PetakBebasParkir(
                        data.indeks,
                        data.kode,
                        data.nama
                    )
                );
            }
        }
    }
}

Petak* Papan::getPetak(int indeks){
    if(indeks < 1 || indeks > getTotalPetak()) return nullptr;
    return daftarPetak[indeks - 1];
}

Petak* Papan::getPetak(const std::string& kode){
    for(Petak* petak : daftarPetak){
        if(petak->getKode() == kode){
            return petak;
        }
    }
    return nullptr;
}

// Mencari petak dengan kode tertentu yang terdekat dari pemain pada indeks tertentu
Petak* Papan::findNearestByKode(const std::string& kode, int indeks){
    for(int i = 0; i < getTotalPetak(); i++){
        int idx = (indeks - 1 + i) % getTotalPetak();

        if(daftarPetak.at(idx)->getKode() == kode){
            return daftarPetak.at(idx);
        }
    }

    // Kalau tidak ditemukan sama sekali : error / exception mungkin
    return nullptr;
}

int Papan::getTotalPetak(){
    return daftarPetak.size();
}

Papan::~Papan(){
    for(auto p : daftarPetak){
        delete p;
    }
}