#include "utils/ConfigLoader.hpp"
#include "utils/NimonspoliException.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

int main(){
    try {
        cout << "Loading config from 'config/' folder..." << endl;
        ConfigData config = ConfigLoader::loadAll("config");
        
        cout << "\n=== TAX DATA ===" << endl;
        cout << "PPH Flat: " << config.getPphFlat() << endl;
        cout << "PPH Persen: " << config.getPphPersen() << endl;
        cout << "PBM Flat: " << config.getPbmFlat() << endl;
        
        cout << "\n=== SPECIAL DATA ===" << endl;
        cout << "Gaji GO: " << config.getGajiGo() << endl;
        cout << "Denda Penjara: " << config.getDendaPenjara() << endl;
        
        cout << "\n=== MISC DATA ===" << endl;
        cout << "Max Turn: " << config.getMaxTurn() << endl;
        cout << "Uang Awal Pemain: " << config.getUangAwalPemain() << endl;
        
        cout << "\n=== RAILROAD DATA ===" << endl;
        auto& rrMap = config.getHargaSewaRailroad();
        cout << "Jumlah entry: " << rrMap.size() << endl;
        for (const auto& entry : rrMap) {
            cout << "  " << entry.first << " railroad -> " << entry.second << " sewa" << endl;
        }
        
        cout << "\n=== UTILITY DATA ===" << endl;
        auto& utilMap = config.getPengaliUtility();
        cout << "Jumlah entry: " << utilMap.size() << endl;
        for (const auto& entry : utilMap) {
            cout << "  " << entry.first << " utility -> faktor " << entry.second << endl;
        }
        
        cout << "\n=== PROPERTY DATA ===" << endl;
        auto& propMap = config.getPropertiMap();
        cout << "Jumlah property: " << propMap.size() << endl;
        cout << "\nSample properties:" << endl;
        int count = 0;
        for (const auto& entry : propMap) {
            if (count >= 3) break;
            const PropertiConfig& prop = entry.second;
            cout << "\n  Kode: " << prop.getKode() << endl;
            cout << "  Nama: " << prop.getNama() << endl;
            cout << "  Jenis: " << prop.getJenis() << endl;
            cout << "  Warna: " << prop.getWarna() << endl;
            cout << "  Harga Lahan: " << prop.getHargaLahan() << endl;
            cout << "  Nilai Gadai: " << prop.getNilaiGadai() << endl;
            cout << "  Harga Upgrade Rumah: " << prop.getHargaBangunRumah() << endl;
            cout << "  Harga Upgrade Hotel: " << prop.getHargaBangunHotel() << endl;
            cout << "  Harga Sewa L0-L5: ";
            for (int i = 0; i < 6; i++) {
                cout << prop.getHargaSewa()[i];
                if (i < 5) cout << ", ";
            }
            cout << endl;
            count++;
        }

        cout << "\n=== AKSI DATA ===" << endl;
        auto& aksiMap = config.getAksiMap();
        cout << "Jumlah petak aksi: " << aksiMap.size() << endl;
        cout << "\nSample petak aksi:" << endl;
        count = 0;
        for (const auto& entry : aksiMap) {
            if (count >= 3) break;
            const AksiConfig& aksi = entry.second;
            cout << "\n  Kode: " << aksi.getKode() << endl;
            cout << "  Nama: " << aksi.getNama() << endl;
            cout << "  Jenis: " << aksi.getJenis() << endl;
            cout << "  Warna: " << aksi.getWarna() << endl;
            count++;
        }
        
        cout << "\n=== CONFIG LOAD SUCCESS ===" << endl;
        return 0;
        
    } catch (const FileTidakValidException& e) {
        cerr << "Error: File tidak valid - " << e.what() << endl;
        cerr << "Error Code: " << e.getkodeError() << endl;
        return 1;
    } catch (const NimonspoliException& e) {
        cerr << "Error: " << e.what() << endl;
        cerr << "Error Code: " << e.getkodeError() << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "Unexpected error: " << e.what() << endl;
        return 1;
    }
}