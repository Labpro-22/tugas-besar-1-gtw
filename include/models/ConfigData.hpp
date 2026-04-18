#include <map>
#include <vector>
#include <string>
#include <utility>

class PropertiConfig{
private:
    int id;
    std::string kode;
    std::string nama;
    std::string jenis;
    std::string warna;
    int hargaLahan;
    int nilaiGadai;

    int hargaBangunRumah;
    int hargaBangunHotel;
    std::vector<int> hargaSewa; //untuk setiap prop level
public:
    //default ctor
    PropertiConfig();
    
    //ctor
    PropertiConfig(int id, std::string kode, std::string nama, std::string jenis, std::string warna, int hargaLahan,
                   int nilaiGadai, int hargaBangunRumah, int hargaBangunHotel, std::vector<int> hargaSewa);

    //getter
    int getId() const;
    const std::string &getKode() const;
    const std::string &getNama() const;
    const std::string &getJenis() const;
    const std::string &getWarna() const;
    int getHargaLahan() const;
    int getNilaiGadai() const;
    int getHargaBangunRumah() const;
    int getHargaBangunHotel() const;

    const std::vector<int> &getHargaSewa() const;
    int getHargaSewa(int level) const;
};

class ConfigData{
private:
    std::map<std::string, PropertiConfig> propertiMap; //untuk setiap prop kode <-> its detail
    std::map<int, int> hargaSewaRailroad; //untuk setiap jumlahRailroad
    std::map<int, int> pengaliUtility; //untuk setiap jumlahUtility
    int pphFlat;
    int pphPersen;
    int pbmFlat;
    int gajiGo;
    int dendaPenjara;
    int maxTurn;
    int uangAwalPemain;
public:
    //ctor
    ConfigData(int pphFlat, int pphPersen, int pbmFlat, int gajiGo, int dendaPenjara,
               int maxTurn, int uangAwal, std::map<std::string, PropertiConfig> propertiMap,
               std::map<int, int> sewaRailroad, std::map<int, int> pengaliUtility);

    //getter
    const std::map<std::string, PropertiConfig> &getPropertiMap() const;
    const PropertiConfig &getPropertiMap(const std::string &kodeProperti) const;

    const std::map<int, int> &getHargaSewaRailroad() const;
    int getHargaSewaRailroad(int jumlahRailroad) const;

    const std::map<int, int> &getPengaliUtility() const;
    int getPengaliUtility(int jumlahUtility) const; 

    int getPphFlat() const;
    int getPphPersen() const;
    int getPbmFlat() const;
    int getGajiGo() const;
    int getDendaPenjara() const;
    int getMaxTurn() const;
    int getUangAwalPemain() const;
};