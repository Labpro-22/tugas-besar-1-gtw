#include "models/Pemain.hpp"
#include "models/Petak/PetakProperti.hpp"
#include "models/Petak/PetakStasiun.hpp"
#include "models/Petak/PetakUtilitas.hpp"
#include "models/PlayerActionService.hpp"
#include "views/OutputHandler.hpp"
#include <iostream>

PetakProperti::PetakProperti(int indeks, const std::string& kode, const std::string& nama, const PropertiConfig& config) : 
    Petak(indeks, kode, nama), config(&config), pemilik(nullptr), status(StatusProperti::BANK) {}

void PetakProperti::onLanded(Pemain& pemain, PlayerActionService& actionService) {
    std::cout << "Mendarat di properti: " << nama << " (" << kode << ")\n";

    // Tidak ada aksi jika mendarat di properti sendiri.
    if (pemilik == &pemain) {
        return;
    }

    if (status == StatusProperti::BANK) {
        // Railroad dan Utility: kepemilikan otomatis ke pemain pertama yang mendarat.
        if (dynamic_cast<PetakStasiun*>(this) || dynamic_cast<PetakUtilitas*>(this)) {
            setPemilik(&pemain);
            pemain.tambahAset(this);
            OutputHandler::cetakPesan("Belum ada yang menginjaknya duluan, " + nama + " kini menjadi milikmu!");
            actionService.logAksi(pemain, "BELI_OTOMATIS", kode + " kini milik " + pemain.getUsername());
            return;
        }

        // Street: pemain ditawari beli, jika tidak maka masuk lelang (ditangani service).
        actionService.beliProperti(pemain, *this);
        return;
    }

    if (status == StatusProperti::OWNED && pemilik != nullptr) {
        int nilaiDadu = 0;
        if (actionService.getDadu()) {
            nilaiDadu = actionService.getDadu()->getTotalNilaiDadu();
        }
        actionService.bayarSewa(pemain, *this, nilaiDadu);
    }
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