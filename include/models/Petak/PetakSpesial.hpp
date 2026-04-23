#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include "Petak.hpp"
#include "PetakAksi.hpp"
#include "../Pemain.hpp"
#include "../ConfigData.hpp"
#include "../Dadu.hpp"
#include "../PlayerActionService.hpp"

using namespace std;

class PetakSpesial : public PetakAksi {
public :
    PetakSpesial::PetakSpesial(int indeks, const std::string& kode, const std::string& nama);
    virtual void onLanded (Pemain &p, PlayerActionService actionService) = 0;

};

class PetakMulai : public PetakSpesial {
public : 
    PetakMulai::PetakMulai(int indeks, const std::string& kode, const std::string& nama);
    void onLanded (Pemain &p, PlayerActionService actionService) override;
};


class PetakPenjara : public PetakSpesial {
protected : 
    ManagerPenjara* manager_penjara;
public : 
    PetakPenjara::PetakPenjara(int indeks, const std::string& kode, const std::string& nama);
    void onLanded (Pemain &p, PlayerActionService actionService) override;


};

class PetakPergiPenjara : public PetakSpesial {
public : 
    PetakPergiPenjara::PetakPergiPenjara(int indeks, const std::string& kode, const std::string& nama);
    void onLanded (Pemain &p, PlayerActionService actionService) override;
    void masukkanKePenjara (Pemain &p, PlayerActionService actionService);

};

class PetakBebasParkir : public PetakSpesial {
public : 
    PetakBebasParkir::PetakBebasParkir(int indeks, const std::string& kode, const std::string& nama);
    void onLanded (Pemain &p, PlayerActionService actionService) override;

};
