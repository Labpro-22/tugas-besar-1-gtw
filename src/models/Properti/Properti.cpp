#include "models/Properti/Properti.hpp"
#include <string>

Properti::Properti(const PropertiConfig& config) :
    config(&config), pemilik(nullptr), status(StatusProperti::BANK){}

const std::string& Properti::getKode() const{
    return config->getKode();
}

const std::string& Properti::getNamaProperti() const{
    return config->getNama();
}

Properti::StatusProperti Properti::getStatus() const{
    return status;
}

std::string Properti::getJenisString() const{
    return config->getJenis();
}

std::string Properti::getBangunanString() const{
    return "0";
}

std::string Properti::getStatusString() const{
    switch (status) {
    case StatusProperti::BANK:
        return "BANK";
    case StatusProperti::OWNED:
        return "OWNED";
    case StatusProperti::MORTGAGED:
        return "MORTGAGED";
    }

    return "BANK";
}

std::string Properti::getPemilikString() const{
    if (pemilik == nullptr) {
        return "BANK";
    }

    return pemilik->getUsername();
}

Pemain* Properti::getPemilik() const{
    return pemilik;
}

void Properti::setPemilik(Pemain* pemain){
    pemilik = pemain;
    status = StatusProperti::OWNED;
}

void Properti::gadai(){
    status = StatusProperti::MORTGAGED;
}

void Properti::batalGadai(){
    status = StatusProperti::OWNED;
}

int Properti::getNilaiGadai() const {
    return config->getNilaiGadai();
}