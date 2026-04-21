#include "models/Properti/Properti.hpp"
#include "models/Properti/ManagerProperti.hpp"
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