#include "models/Petak/PetakProperti.hpp"

PetakProperti::PetakProperti(int indeks, const std::string& kode, const std::string& nama, Properti* properti) : 
    Petak(indeks, kode, nama), properti(properti){}