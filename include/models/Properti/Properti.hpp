#pragma once
#include <string>
#include "../Pemain.hpp"
#include "../ConfigData.hpp"

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

    virtual std::string getJenisString() const;
    virtual std::string getBangunanString() const;
    std::string getStatusString() const;
    std::string getPemilikString() const;

    Pemain* getPemilik() const;

    const PropertiConfig* getConfigProperti (); 

    void setPemilik(Pemain* pemilik);

    void setStatus (StatusProperti s);

    void gadai();

    void batalGadai();

    int getNilaiGadai() const;

    virtual ~Properti() = default;

protected:
    const PropertiConfig* config;
    Pemain* pemilik;
    StatusProperti status;

};