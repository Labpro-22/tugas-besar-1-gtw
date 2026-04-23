#pragma once
#include "../models/ConfigData.hpp"
#include <string>

class ConfigLoader{
public:
    static ConfigData loadAll(const std::string& folderPath);
private:
    static std::map<int, PropertiConfig> loadProperty(const std::vector<std::string>& lines);
    static std::map<int, AksiConfig> loadAksi(const std::vector<std::string>& lines);
    static std::map<int, int> loadRailroad(const std::vector<std::string>& lines);
    static std::map<int, int> loadUtility(const std::vector<std::string>& lines);

    struct TaxData    { int pphFlat, pphPersen, pbmFlat; };
    struct SpecialData{ int gajiGo, dendaPenjara; };
    struct MiscData   { int maxTurn, uangAwal; };

    static TaxData loadTax(const std::vector<std::string>& lines);
    static SpecialData loadSpecial(const std::vector<std::string>& lines);
    static MiscData loadMisc(const std::vector<std::string>& lines);
};