#include "models/Petak/Petak.hpp"

Petak::Petak(int indeks, const std::string& kode, const std::string& nama) : indeks(indeks), kode(kode), nama(nama){}

int Petak::getIndeks() const{
    return indeks;
}

const std::string& Petak::getKode() const{
    return kode;
}

const std::string& Petak::getNama() const{
    return nama;
}

std::string Petak::getDisplayInfo() const{
    return std::to_string(indeks) + " " + kode + " " + nama; 
}