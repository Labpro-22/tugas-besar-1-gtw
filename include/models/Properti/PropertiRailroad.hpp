#pragma once
#include "Properti.hpp"

class PropertiRailroad : public Properti{
public:
    PropertiRailroad(const PropertiConfig& config);
    int hitungSewa(int dadu, ManagerProperti& manager) override;
    std::string getJenisString() const override;
    ~PropertiRailroad() = default;
};