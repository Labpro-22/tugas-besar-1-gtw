#pragma once
// include petak.hpp
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include "../models/Pemain.hpp"
#include "../models/ConfigData.hpp"
#include "../models/Dadu.hpp"

using namespace std;

class PetakSpesial {
public :
    virtual void onLanded (Pemain *p/*, playerActionService*/) = 0;

};

class PetakMulai : public PetakSpesial {
public : 
    void onLanded (Pemain *p/*, playerActionService*/) override;
};


class PetakPenjara : public PetakSpesial {
protected : 
    ManagerPenjara* manager_penjara;
public : 
    void onLanded (Pemain *p/*, playerActionService*/) override;


};

class PetakPergiPenjara : public PetakSpesial {
public : 
    void onLanded (Pemain *p/*, playerActionService*/) override;
    void masukkanKePenjara (Pemain *p);

};

class PetakBebasParkir : public PetakSpesial {
public : 
    void onLanded (Pemain *p/*, playerActionService*/) override;

};

class ManagerPenjara {
protected :
    map<Pemain*, int> jumlahPercobaan;
public :
    void tryEscapeByDouble (Pemain* p, Dadu* d);

    void escapeByFine ();

    void paksaBayar (); // udah turn4
};