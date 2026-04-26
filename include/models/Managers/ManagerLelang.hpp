#pragma once
#include <vector>

// Forward declarations
class PetakProperti;
class Pemain;
class ManagerTransaksi;

class ManagerLelang {
private:
    ManagerTransaksi* managerTransaksi;

public:
    explicit ManagerLelang(ManagerTransaksi* mt) : managerTransaksi(mt) {}
    ~ManagerLelang() = default;

    /**
     * Memulai proses lelang untuk properti tertentu.
     * @param properti Properti yang akan dilelang
     * @param daftarPemain Seluruh pemain di game
     * @param pemainPemicu Pemain yang memicu lelang (lelang dimulai dari pemain SETELAH pemain ini)
     */
    void mulaiLelang(PetakProperti* properti, const std::vector<Pemain*>& daftarPemain, Pemain* pemainPemicu);
};
