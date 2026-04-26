#pragma once
#include "Properti.hpp"

class PropertiUtility : public Properti{
public:
    PropertiUtility(const PropertiConfig& config);
    int hitungSewa(int dadu, ManagerProperti& manager) override;
    std::string getJenisString() const override;
    ~PropertiUtility() = default;
}; 