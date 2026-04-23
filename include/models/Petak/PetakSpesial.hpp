#include "Petak.hpp"

class PetakSpesial : public Petak{
public:
    PetakSpesial(int indeks, const std::string& kode, const std::string& nama);

    virtual void onLanded(Pemain& pemain, PlayerActionService& actionService);

    virtual ~PetakSpesial() = default;
};