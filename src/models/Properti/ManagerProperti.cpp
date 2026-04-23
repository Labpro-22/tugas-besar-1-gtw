#include "models/Properti/ManagerProperti.hpp"

ManagerProperti::ManagerProperti(const ConfigData& configData) : 
    configData(configData){
        for(auto& pair : configData.getPropertiMap()){
        const PropertiConfig& cfg = pair.second;

        if(cfg.getJenis() == "STREET"){
            daftarProperti[cfg.getId()] = new PropertiStreet(cfg);
        }
        else if(cfg.getJenis() == "RAILROAD"){
            daftarProperti[cfg.getId()] = new PropertiRailroad(cfg);
        }
        else if(cfg.getJenis() == "UTILITY"){
            daftarProperti[cfg.getId()] = new PropertiUtility(cfg);
        }
    }
    }

bool ManagerProperti::isMonopoly(Pemain* pemain, PropertiStreet::ColorGroup warna){
    auto list = getPropertiByGrup(warna);

    if(list.empty()) return false;

    for(auto p : list){
        if(p->getPemilik() != pemain || 
           p->getStatus() != Properti::StatusProperti::OWNED){
            return false;
        }
    }

    return true;
}

bool ManagerProperti::isBisaBangun(PropertiStreet* target){
    // tidak bisa bangun kalau pemain tidak memonopoli
    if(!isMonopoly(target->getPemilik(), target->getWarna())){
        return false;
    }

    auto group = getPropertiByGrup(target->getWarna());
    int targetLevel = target->getJumlahBangunan();

    // tidak dapat membangun jika selisih rumah antarpetak lebih dari 1
    for(auto p : group){
        if(abs(p->getJumlahBangunan() - targetLevel) > 1){
            return false;
        }
    }

    return true;
}

void ManagerProperti::jualSemuaBangunan(Pemain* pemain, PropertiStreet::ColorGroup warna){
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
        Properti* p = pair.second;

        if(p->getPemilik() != pemain) continue;

        // nilai gadai
        total += p->getNilaiGadai();

        // bangunan (khusus street)
        auto street = dynamic_cast<PropertiStreet*>(p);
        if(street){
            int level = street->getJumlahBangunan();
            total += level * (street->getHargaBangun() / 2);
        }
    }

    return total;
}

bool ManagerProperti::isPunyaSemuaDalamGrup(Pemain* pemain, PropertiStreet::ColorGroup warna){
    auto group = getPropertiByGrup(warna);

    if(group.empty()) return false;

    for(auto p : group){
        if(p->getPemilik() != pemain){
            return false;
        }
    }

    return true;
}

std::vector<PropertiStreet*> ManagerProperti::getPropertiByGrup(PropertiStreet::ColorGroup warna){
    std::vector<PropertiStreet*> result;

    for(auto& pair : daftarProperti){
        auto street = dynamic_cast<PropertiStreet*>(pair.second);

        if(street && street->getWarna() == warna){
            result.push_back(street);
        }
    }

    return result;
}

Properti* ManagerProperti::getProperti(int id){
    auto it = daftarProperti.find(id);
    if(it != daftarProperti.end()){
        return it->second;
    }
    return nullptr;
}

int ManagerProperti::hitungUtility(Pemain* pemain){
    int count = 0;

    for(auto& pair : daftarProperti){
        auto util = dynamic_cast<PropertiUtility*>(pair.second);

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
        auto rr = dynamic_cast<PropertiRailroad*>(pair.second);

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
    for(auto& p : daftarProperti){
        delete p.second;
    }
}

