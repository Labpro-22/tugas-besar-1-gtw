#include "models/Papan.hpp"

Papan::Papan(ManagerProperti& manager, ConfigData& configData){
    int n = configData.getPropertiMap().size() + configData.getAksiMap().size();

    for(int i = 1; i <= n; i++){
        // PROPERTY (Street, Railroad, Utility)
        const auto& propertiMap = configData.getPropertiMap();
        auto properti = propertiMap.find(i);
        if(properti != propertiMap.end()){ //id merupakan properti
            Properti* p = manager.getProperti(properti->second.getId());
            daftarPetak.push_back(
                new PetakProperti(
                    properti->second.getId(),
                    properti->second.getKode(),
                    properti->second.getNama(),
                    p
                )
            );
        }

        // Aksi
        const auto& aksiMap = configData.getAksiMap();
        auto aksi = aksiMap.find(i);
        if(aksi != aksiMap.end()){ //id merupakan aksi
            if(aksi->second.getJenis() == "KARTU"){
                daftarPetak.push_back(
                    new PetakKartu(
                        aksi->second.getId(),
                        aksi->second.getKode(),
                        aksi->second.getNama()
                    )
                );
            }
        
            else if(aksi->second.getJenis() == "PAJAK"){
                daftarPetak.push_back(
                    new PetakPajak(
                        aksi->second.getId(),
                        aksi->second.getKode(),
                        aksi->second.getNama()
                    )
                );
            }

            else if(aksi->second.getJenis() == "FESTIVAL"){
                daftarPetak.push_back(
                    new PetakFestival(
                        aksi->second.getId(),
                        aksi->second.getKode(),
                        aksi->second.getNama()
                    )
                );
            }

            // SPESIAL 
            else if(aksi->second.getJenis() == "SPESIAL"){
                if(aksi->second.getKode() == "GO"){
                    daftarPetak.push_back(
                        new PetakMulai(
                            aksi->second.getId(),
                            aksi->second.getKode(),
                            aksi->second.getNama()
                        )
                    );
                }
                else if(aksi->second.getKode() == "PEN"){
                    daftarPetak.push_back(
                        new PetakPenjara(
                            aksi->second.getId(),
                            aksi->second.getKode(),
                            aksi->second.getNama()
                        )
                    );
                }
                else if(aksi->second.getKode() == "PPJ"){
                    daftarPetak.push_back(
                        new PetakPergiKePenjara(
                            aksi->second.getId(),
                            aksi->second.getKode(),
                            aksi->second.getNama()
                        )
                    );
                }
                else if(aksi->second.getKode() == "BBP"){
                    daftarPetak.push_back(
                        new PetakBebasParkir(
                            aksi->second.getId(),
                            aksi->second.getKode(),
                            aksi->second.getNama()
                        )
                    );
                }
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

int Papan::getTotalPetak() const {
    return daftarPetak.size();
}

Papan::~Papan(){
    for(auto p : daftarPetak){
        delete p;
    }
}