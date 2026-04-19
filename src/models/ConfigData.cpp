#include "models/ConfigData.hpp"
#include <utility>

using namespace std;

PropertiConfig::PropertiConfig()
    : id(0), kode(""), nama(""), jenis(""), warna(""),
      hargaLahan(0), nilaiGadai(0), hargaBangunRumah(0),
      hargaBangunHotel(0), hargaSewa(6, 0) {}

PropertiConfig::PropertiConfig(int id, string kode, string nama, string jenis, string warna, int hargaLahan,
                               int nilaiGadai, int hargaBangunRumah, int hargaBangunHotel, vector<int> hargaSewa)
    : id(id), kode(move(kode)), nama(move(nama)), jenis(move(jenis)), warna(move(warna)),
      hargaLahan(hargaLahan), nilaiGadai(nilaiGadai), hargaBangunRumah(hargaBangunRumah),
      hargaBangunHotel(hargaBangunHotel), hargaSewa(move(hargaSewa)) {}

int PropertiConfig::getId() const { return id; }
const string &PropertiConfig::getKode() const { return kode; }
const string &PropertiConfig::getNama() const { return nama; }
const string &PropertiConfig::getJenis() const { return jenis; }
const string &PropertiConfig::getWarna() const { return warna; }
int PropertiConfig::getHargaLahan() const { return hargaLahan; }
int PropertiConfig::getNilaiGadai() const { return nilaiGadai; }
int PropertiConfig::getHargaBangunRumah() const { return hargaBangunRumah; }
int PropertiConfig::getHargaBangunHotel() const { return hargaBangunHotel; }
const vector<int> &PropertiConfig::getHargaSewa() const { return hargaSewa; }
int PropertiConfig::getHargaSewa(int level) const { return hargaSewa.at(level); }

ConfigData::ConfigData(int pphFlat, int pphPersen, int pbmFlat, int gajiGo, int dendaPenjara,
                       int maxTurn, int uangAwal, map<string, PropertiConfig> propertiMap,
                       map<int, int> sewaRailroad, map<int, int> pengaliUtility)
    : propertiMap(move(propertiMap)), hargaSewaRailroad(move(sewaRailroad)),
      pengaliUtility(move(pengaliUtility)), pphFlat(pphFlat), pphPersen(pphPersen), pbmFlat(pbmFlat),
      gajiGo(gajiGo), dendaPenjara(dendaPenjara), maxTurn(maxTurn), uangAwalPemain(uangAwal) {}

const map<string, PropertiConfig> &ConfigData::getPropertiMap() const { return propertiMap; }
const PropertiConfig &ConfigData::getPropertiMap(const string &kodeProperti) const { return propertiMap.at(kodeProperti); }
const map<int, int> &ConfigData::getHargaSewaRailroad() const { return hargaSewaRailroad; }
int ConfigData::getHargaSewaRailroad(int jumlahRailroad) const { return hargaSewaRailroad.at(jumlahRailroad); }
const map<int, int> &ConfigData::getPengaliUtility() const { return pengaliUtility; }
int ConfigData::getPengaliUtility(int jumlahUtility) const { return pengaliUtility.at(jumlahUtility); }
int ConfigData::getPphFlat() const { return pphFlat; }
int ConfigData::getPphPersen() const { return pphPersen; }
int ConfigData::getPbmFlat() const { return pbmFlat; }
int ConfigData::getGajiGo() const { return gajiGo; }
int ConfigData::getDendaPenjara() const { return dendaPenjara; }
int ConfigData::getMaxTurn() const { return maxTurn; }
int ConfigData::getUangAwalPemain() const { return uangAwalPemain; }