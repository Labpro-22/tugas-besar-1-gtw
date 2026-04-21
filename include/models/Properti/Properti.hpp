#pragma once
#include <string>
#include "models/Pemain.hpp"
#include "models/ConfigData.hpp"

class ManagerProperti;

class Properti{
public:
    enum class StatusProperti{
        BANK,
        OWNED,
        MORTGAGED
    };
    Properti(const PropertiConfig& config);

    virtual int hitungSewa(int dadu, ManagerProperti& manager) = 0;

    const std::string& getKode() const;

    const std::string& getNamaProperti() const;

    StatusProperti getStatus() const;

    Pemain* getPemilik() const;

    void setPemilik(Pemain* pemilik);

    void gadai();

    void batalGadai();

    int getNilaiGadai() const;

    virtual ~Properti() = default;

protected:
    const PropertiConfig* config;
    Pemain* pemilik;
    StatusProperti status;

};