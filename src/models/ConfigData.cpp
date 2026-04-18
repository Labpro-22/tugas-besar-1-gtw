#include "models/ConfigData.hpp"

PropertiConfig::PropertiConfig()
    : id(0), kode(""), nama(""), jenis(""), warna(""),
      hargaLahan(0), nilaiGadai(0), hargaBangunRumah(0),
      hargaBangunHotel(0), hargaSewa(6, 0) {}

PropertiConfig::PropertiConfig(int id, std::string kode, std::string nama, std::string jenis, std::string warna, int hargaLahan,
                               int nilaiGadai, int hargaBangunRumah, int hargaBangunHotel, std::vector<int> hargaSewa)
    : id(id), kode(std::move(kode)), nama(std::move(nama)), jenis(std::move(jenis)), warna(std::move(warna)),
      hargaLahan(hargaLahan), nilaiGadai(nilaiGadai), hargaBangunRumah(hargaBangunRumah),
      hargaBangunHotel(hargaBangunHotel), hargaSewa(std::move(hargaSewa)) {}

int PropertiConfig::getId() const { return id; }
const std::string &PropertiConfig::getKode() const { return kode; }
const std::string &PropertiConfig::getNama() const { return nama; }
const std::string &PropertiConfig::getJenis() const { return jenis; }
const std::string &PropertiConfig::getWarna() const { return warna; }
int PropertiConfig::getHargaLahan() const { return hargaLahan; }
int PropertiConfig::getNilaiGadai() const { return nilaiGadai; }
int PropertiConfig::getHargaBangunRumah() const { return hargaBangunRumah; }
int PropertiConfig::getHargaBangunHotel() const { return hargaBangunHotel; }
const std::vector<int> &PropertiConfig::getHargaSewa() const { return hargaSewa; }
int PropertiConfig::getHargaSewa(int level) const { return hargaSewa.at(level); }

ConfigData::ConfigData(int pphFlat, int pphPersen, int pbmFlat, int gajiGo, int dendaPenjara,
                       int maxTurn, int uangAwal, std::map<std::string, PropertiConfig> propertiMap,
                       std::map<int, int> sewaRailroad, std::map<int, int> pengaliUtility)
    : propertiMap(std::move(propertiMap)), hargaSewaRailroad(std::move(sewaRailroad)),
      pengaliUtility(std::move(pengaliUtility)), pphFlat(pphFlat), pphPersen(pphPersen), pbmFlat(pbmFlat),
      gajiGo(gajiGo), dendaPenjara(dendaPenjara), maxTurn(maxTurn), uangAwalPemain(uangAwal) {}

const std::map<std::string, PropertiConfig> &ConfigData::getPropertiMap() const { return propertiMap; }
const PropertiConfig &ConfigData::getPropertiMap(const std::string &kodeProperti) const { return propertiMap.at(kodeProperti); }
const std::map<int, int> &ConfigData::getHargaSewaRailroad() const { return hargaSewaRailroad; }
int ConfigData::getHargaSewaRailroad(int jumlahRailroad) const { return hargaSewaRailroad.at(jumlahRailroad); }
const std::map<int, int> &ConfigData::getPengaliUtility() const { return pengaliUtility; }
int ConfigData::getPengaliUtility(int jumlahUtility) const { return pengaliUtility.at(jumlahUtility); }
int ConfigData::getPphFlat() const { return pphFlat; }
int ConfigData::getPphPersen() const { return pphPersen; }
int ConfigData::getPbmFlat() const { return pbmFlat; }
int ConfigData::getGajiGo() const { return gajiGo; }
int ConfigData::getDendaPenjara() const { return dendaPenjara; }
int ConfigData::getMaxTurn() const { return maxTurn; }
int ConfigData::getUangAwalPemain() const { return uangAwalPemain; }