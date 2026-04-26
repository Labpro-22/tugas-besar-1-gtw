#include "models/Dadu.hpp"
#include <random> 

Dadu::Dadu() : d1(1), d2(1),
    rng(std::random_device{}()) {} //seed sekali di constructor

void Dadu::rollRandom() {
    std::uniform_int_distribution<int> dist(1, 6); //1-6 inklusif
    d1 = dist(rng);
    d2 = dist(rng);
}
 
void Dadu::rollManual(int v1, int v2){
    d1 = v1;
    d2 = v2;
}