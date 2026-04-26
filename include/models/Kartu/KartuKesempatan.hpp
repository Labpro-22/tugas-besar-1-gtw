#pragma once
#include "Kartu.hpp"

class KartuKesempatan : public Kartu {
public:
    explicit KartuKesempatan(const std::string& nama) : Kartu(nama) {}
    virtual ~KartuKesempatan() = default;
    virtual void jalankanEfek(Pemain& pemain, PlayerActionService& svc) = 0;
};

class KartuPergiKeStasiunTerdekat : public KartuKesempatan {
public:
    KartuPergiKeStasiunTerdekat() : KartuKesempatan("Pergi ke stasiun terdekat.") {}
    ~KartuPergiKeStasiunTerdekat() = default;
    void jalankanEfek(Pemain& pemain, PlayerActionService& svc) override;
};

class KartuMundurTigaPetak : public KartuKesempatan {
public:
    KartuMundurTigaPetak() : KartuKesempatan("Mundur 3 petak.") {}
    ~KartuMundurTigaPetak() = default;
    void jalankanEfek(Pemain& pemain, PlayerActionService& svc) override;
};

class KartuMasukPenjara : public KartuKesempatan {
public:
    KartuMasukPenjara() : KartuKesempatan("Masuk Penjara.") {}
    ~KartuMasukPenjara() = default;
    void jalankanEfek(Pemain& pemain, PlayerActionService& svc) override;
};

class KartuBebasPenjara : public KartuKesempatan {
public:
    KartuBebasPenjara() : KartuKesempatan("Bebas dari Penjara.") {}
    ~KartuBebasPenjara() = default;
    void jalankanEfek(Pemain& pemain, PlayerActionService& svc) override;
    bool harusDisimpan() const { return true; }
};