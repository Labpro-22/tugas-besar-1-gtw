#pragma once
#include <string>
class PlayerActionService;
class Pemain;


class Petak{
protected:
    int indeks;
    std::string kode;
    std::string nama;

public:
    Petak(int indeks, const std::string& kode, const std::string& nama);
    
    int getIndeks() const;

    const std::string& getKode() const;

    const std::string& getNama() const;

    std::string getDisplayInfo() const;

    virtual void onLanded(Pemain& pemain, PlayerActionService& actionService) = 0;

    virtual ~Petak() = default;

};