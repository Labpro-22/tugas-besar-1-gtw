#pragma once
#include <string>

// forward declarations
class Pemain;
class PlayerActionService;

class Kartu {
protected:
    std::string namaKartu;
public:
    explicit Kartu(const std::string& nama) : namaKartu(nama) {}
    virtual ~Kartu() = default;
    virtual void jalankanEfek(Pemain& pemain, PlayerActionService& svc) = 0;
    const std::string& getNamaKartu() const { return namaKartu; }
};
