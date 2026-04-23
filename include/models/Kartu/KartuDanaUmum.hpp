#pragma once
#include "Kartu.hpp"

class KartuDanaUmum : public Kartu {
public:
    explicit KartuDanaUmum(const std::string& nama) : Kartu(nama) {}
    virtual ~KartuDanaUmum() = default;
    virtual void jalankanEfek(Pemain& pemain, PlayerActionService& svc) = 0;
};

class KartuHariUlangTahun : public KartuDanaUmum {
public:
    static constexpr int JUMLAH = 100;
    KartuHariUlangTahun() : KartuDanaUmum("Ini adalah hari ulang tahun Anda. Dapatkan M100 dari setiap pemain") {}
    ~KartuHariUlangTahun() = default;
    void jalankanEfek(Pemain& pemain, PlayerActionService& svc) override;
};

class KartuBiayaDokter : public KartuDanaUmum {
public:
    static constexpr int JUMLAH = 700;
    KartuBiayaDokter() : KartuDanaUmum("Biaya dokter, bayar M700") {}
    ~KartuBiayaDokter() = default;
    void jalankanEfek(Pemain& pemain, PlayerActionService& svc) override;
};

class KartuMauNyaleg : public KartuDanaUmum {
public:
    static constexpr int JUMLAH = 200;
    KartuMauNyaleg() : KartuDanaUmum("Anda mau nyaleg, bayar M200 kepada setiap pemain") {}
    ~KartuMauNyaleg() = default;
    void jalankanEfek(Pemain& pemain, PlayerActionService& svc) override;
};
