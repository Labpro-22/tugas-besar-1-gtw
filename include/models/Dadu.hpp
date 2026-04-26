#pragma once
#include <random>

class Dadu {
private:
    int d1;
    int d2;
    std::mt19937 rng;

public:
    Dadu();

    void rollRandom();                        
    void rollManual(int v1, int v2);           
    
    int getNilaid1() const { return d1; }
    int getNilaid2() const { return d2; }
    int getTotalNilaiDadu()  const { return d1 + d2; }
    bool isDouble() const { return d1 == d2; }
};