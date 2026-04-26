#include "models/Pemain.hpp"
#include "models/Managers/ManagerProperti.hpp"

ManagerProperti::ManagerProperti(const ConfigData& configData) : 
    configData(configData) {
        // Initialization without instantiation.
        // Papan is responsible for creating Petak objects and registering them here.
}

void ManagerProperti::registerProperti(PetakProperti* p) {
    if (p) {
        daftarProperti[p->getIndeks()] = p;
    }
}

bool ManagerProperti::isMonopoly(Pemain* pemain, std::string warna){
    auto list = getPropertiByGrup(warna);

    if(list.empty()) return false;

    for(auto p : list){
        if(p->getPemilik() != pemain || 
           p->getStatus() != PetakProperti::StatusProperti::OWNED){
            return false;
        }
    }

    return true;
}

bool ManagerProperti::isBisaBangun(PetakLahan* target){
    // tidak bisa bangun kalau pemain tidak memonopoli
    if(!isMonopoly(target->getPemilik(), target->getWarnaString())){
        return false;
    }

    auto group = getPropertiByGrup(target->getWarnaString());
    int targetLevel = target->getJumlahBangunan();

    // tidak dapat membangun jika selisih rumah antarpetak lebih dari 1
    for(auto p : group){
        if(abs(p->getJumlahBangunan() - targetLevel) > 1){
            return false;
        }
    }

    return true;
}

void ManagerProperti::jualSemuaBangunan(Pemain* pemain, std::string warna){
    auto group = getPropertiByGrup(warna);

    for(auto p : group){
        if(p->getPemilik() == pemain){
            while(p->getJumlahBangunan() > 0){
                p->turunkanBangunan();
            }
        }
    }
}

int ManagerProperti::hitungLikuidasi(Pemain* pemain){
    int total = 0;

    for(auto& pair : daftarProperti){
        PetakProperti* p = pair.second;

        if(p->getPemilik() != pemain) continue;

        // nilai gadai
        total += p->getNilaiGadai();

        // bangunan (khusus street)
        auto street = dynamic_cast<PetakLahan*>(p);
        if(street){
            int level = street->getJumlahBangunan();
            total += level * (street->getHargaBangun() / 2);
        }
    }

    return total;
}

bool ManagerProperti::isPunyaSemuaDalamGrup(Pemain* pemain, std::string warna){
    auto group = getPropertiByGrup(warna);

    if(group.empty()) return false;

    for(auto p : group){
        if(p->getPemilik() != pemain){
            return false;
        }
    }

    return true;
}

std::vector<PetakLahan*> ManagerProperti::getPropertiByGrup(std::string warna){
    std::vector<PetakLahan*> result;

    for(auto& pair : daftarProperti){
        auto street = dynamic_cast<PetakLahan*>(pair.second);

        if(street && street->getWarnaString() == warna){
            result.push_back(street);
        }
    }

    return result;
}

PetakProperti* ManagerProperti::getProperti(int id){
    auto it = daftarProperti.find(id);
    if(it != daftarProperti.end()){
        return it->second;
    }
    return nullptr;
}

int ManagerProperti::hitungUtility(Pemain* pemain){
    int count = 0;

    for(auto& pair : daftarProperti){
        auto util = dynamic_cast<PetakUtilitas*>(pair.second);

        if(util && util->getPemilik() == pemain){
            count++;
        }
    }

    return count;
}

int ManagerProperti::getPengaliUtility(int jumlah){
    return configData.getPengaliUtility(jumlah);
}

int ManagerProperti::hitungRailroad(Pemain* pemain){
    int count = 0;

    for(auto& pair : daftarProperti){
        auto rr = dynamic_cast<PetakStasiun*>(pair.second);

        if(rr && rr->getPemilik() == pemain){
            count++;
        }
    }

    return count;
}

int ManagerProperti::getHargaSewaRailroad(int jumlah){
    return configData.getHargaSewaRailroad(jumlah);
}

ManagerProperti::~ManagerProperti(){
    // Ownership PetakProperti ada di Papan (Papan::~Papan menghapus semua Petak*).
    // ManagerProperti hanya meregister pointer untuk query/perhitungan.
    daftarProperti.clear();
}



std::vector<PetakLahan*> ManagerProperti::getPropertiBisaDihancurkan(Pemain* pemain) {
    std::vector<PetakLahan*> result;
    if (!pemain) return result;
    for (PetakProperti* p : pemain->getAsetPemain()) {
        auto* street = dynamic_cast<PetakLahan*>(p);
        if (street && street->getJumlahBangunan() > 0) {
            result.push_back(street);
        }
    }
    return result;
}

int ManagerProperti::hancurkanSemuaBangunan(PetakLahan* target) {
    if (!target) return 0;
    int count = target->getJumlahBangunan();
    while (target->getJumlahBangunan() > 0) {
        target->turunkanBangunan();
    }
    return count;
}
