#pragma once
#include <string>
#include <vector>

// forward declarations
class Pemain;
class PlayerActionService;

class KartuKemampuanSpesial {
protected:
    std::string namaKartu;
public:
    explicit KartuKemampuanSpesial(const std::string& nama) : namaKartu(nama) {}
    virtual ~KartuKemampuanSpesial() = default;
    virtual void gunakan(Pemain& pemain, PlayerActionService& svc) = 0;
    virtual std::string getDeskripsi() const = 0;
    virtual std::string getNilaiState() const { return ""; }
    virtual std::string getSisaDurasiState() const { return ""; }
    const std::string& getNamaKartu() const { return namaKartu; }
};

class MoveCard : public KartuKemampuanSpesial {
private:
    int langkah;
public:
    explicit MoveCard(int langkah);
    ~MoveCard() = default;
    void gunakan(Pemain& pemain, PlayerActionService& svc) override;
    std::string getDeskripsi() const override;
    std::string getNilaiState() const override;
    int getLangkah() const { return langkah; }
};

class DiscountCard : public KartuKemampuanSpesial {
private:
    int persenDiskon;
    int sisaDurasi;
public:
    explicit DiscountCard(int persen);
    ~DiscountCard() = default;
    void gunakan(Pemain& pemain, PlayerActionService& svc) override;
    std::string getDeskripsi() const override;
    std::string getNilaiState() const override;
    std::string getSisaDurasiState() const override;
    int getPersenDiskon() const { return persenDiskon; }
    int getSisaDurasi() const { return sisaDurasi; }
    void setSisaDurasi(int d) { sisaDurasi = d; }
};

class ShieldCard : public KartuKemampuanSpesial {
public:
    ShieldCard();
    ~ShieldCard() = default;
    void gunakan(Pemain& pemain, PlayerActionService& svc) override;
    std::string getDeskripsi() const override;
};

class TeleportCard : public KartuKemampuanSpesial {
public:
    TeleportCard();
    ~TeleportCard() = default;
    void gunakan(Pemain& pemain, PlayerActionService& svc) override;
    std::string getDeskripsi() const override;
};

class LassoCard : public KartuKemampuanSpesial {
public:
    LassoCard();
    ~LassoCard() = default;
    void gunakan(Pemain& pemain, PlayerActionService& svc) override;
    std::string getDeskripsi() const override;
};

class DemolitionCard : public KartuKemampuanSpesial {
public:
    DemolitionCard();
    ~DemolitionCard() = default;
    void gunakan(Pemain& pemain, PlayerActionService& svc) override;
    std::string getDeskripsi() const override;
};


// kartu bonus

class RotasiKartuCard : public KartuKemampuanSpesial {
public:
    RotasiKartuCard();
    ~RotasiKartuCard() = default;
    void gunakan(Pemain& pemain, PlayerActionService& svc) override;
    std::string getDeskripsi() const override;
};

class ReverseCard : public KartuKemampuanSpesial {
public:
    ReverseCard();
    ~ReverseCard() = default;
    void gunakan(Pemain& pemain, PlayerActionService& svc) override;
    std::string getDeskripsi() const override;
};

class PenjaraKanCard : public KartuKemampuanSpesial {
public:
    PenjaraKanCard();
    ~PenjaraKanCard() = default;
    void gunakan(Pemain& pemain, PlayerActionService& svc) override;
    std::string getDeskripsi() const override;
};