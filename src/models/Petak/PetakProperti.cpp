#include "models/Pemain.hpp"
#include "models/Petak/PetakProperti.hpp"
#include <iostream>

PetakProperti::PetakProperti(int indeks, const std::string& kode, const std::string& nama, const PropertiConfig& config) : 
    Petak(indeks, kode, nama), config(&config), pemilik(nullptr), status(StatusProperti::BANK) {}

void PetakProperti::onLanded(Pemain& pemain, PlayerActionService& actionService) {
    // For now, delegate to PlayerActionService until Phase 3 rewrites this.
    // actionService.handleLandedOnProperti(pemain, *this);
    std::cout << "Mendarat di properti: " << nama << " (" << kode << ")\n";
}

PetakProperti::StatusProperti PetakProperti::getStatus() const {
    return status;
}

void PetakProperti::setStatus(StatusProperti s) {
    status = s;
}

Pemain* PetakProperti::getPemilik() const {
    return pemilik;
}

void PetakProperti::setPemilik(Pemain* pemain) {
    pemilik = pemain;
    status = StatusProperti::OWNED;
}

const PropertiConfig* PetakProperti::getConfigProperti() const {
    return config;
}

void PetakProperti::gadai() {
    status = StatusProperti::MORTGAGED;
}

void PetakProperti::batalGadai() {
    status = StatusProperti::OWNED;
}

int PetakProperti::getNilaiGadai() const {
    return config->getNilaiGadai();
}

std::string PetakProperti::getJenisString() const {
    return config->getJenis();
}

std::string PetakProperti::getBangunanString() const {
    return "0";
}

std::string PetakProperti::getStatusString() const {
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

std::string PetakProperti::getPemilikString() const {
    if (pemilik == nullptr) {
        return "BANK";
    }
    return pemilik->getUsername();
}