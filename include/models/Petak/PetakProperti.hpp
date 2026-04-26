#pragma once
#include "Petak.hpp"
#include "../ConfigData.hpp"
#include <string>

class ManagerProperti;

class PetakProperti : public Petak {
public:
    enum class StatusProperti {
        BANK,
        OWNED,
        MORTGAGED
    };

protected:
    const PropertiConfig* config;
    Pemain* pemilik;
    StatusProperti status;

public:
    PetakProperti(int indeks, const std::string& kode, const std::string& nama, const PropertiConfig& config);

    virtual int hitungSewa(int dadu, ManagerProperti& manager) = 0;

    virtual void onLanded(Pemain& pemain, PlayerActionService& actionService) override;

    StatusProperti getStatus() const;
    void setStatus(StatusProperti s);
    
    Pemain* getPemilik() const;
    void setPemilik(Pemain* pemilik);
    
    const PropertiConfig* getConfigProperti() const;

    void gadai();
    void batalGadai();
    int getNilaiGadai() const;

    virtual std::string getJenisString() const;
    virtual std::string getBangunanString() const;
    std::string getStatusString() const;
    std::string getPemilikString() const;

    virtual ~PetakProperti() = default;
};