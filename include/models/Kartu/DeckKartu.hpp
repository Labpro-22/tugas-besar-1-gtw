#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include <stdexcept>
#include <string>

template <typename T>
class DeckKartu {
private:
    std::vector<T*> tumpukanKartu; // sumber kartu yang dapat diambil
    std::vector<T*> pembuangan; // kartu yang telah digunakan
public:
    DeckKartu() = default;
    ~DeckKartu() = default;
    void tambahKeKartu(T* kartu) {
        tumpukanKartu.push_back(kartu);
    }
    T* ambilKartu() {
        if (tumpukanKartu.empty()) {
            acakUlang();
        }
        if (tumpukanKartu.empty()) {
            throw std::runtime_error("Deck kosong");
        }
        T* kartu = tumpukanKartu.back();
        tumpukanKartu.pop_back();
        return kartu;
    }
    void buangKartu(T* kartu) {
        pembuangan.push_back(kartu);
    }
    void acakUlang() {
        for (T* k : pembuangan) {
            tumpukanKartu.push_back(k);
        }
        pembuangan.clear();
        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(tumpukanKartu.begin(), tumpukanKartu.end(), gen);
    }
    int ukuran() const {
        return static_cast<int>(tumpukanKartu.size());
    }
    int ukuranPembuangan() const {
        return static_cast<int>(pembuangan.size());
    }
    bool kosong() const {
        return tumpukanKartu.empty() && pembuangan.empty();
    }
    const std::vector<T*>& getTumpukan() const { return tumpukanKartu; }
    const std::vector<T*>& getPembuangan() const { return pembuangan; }
};