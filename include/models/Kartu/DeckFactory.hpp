#pragma once
#include "models/Kartu/DeckKartu.hpp"
#include "models/Kartu/Kartu.hpp"
#include "models/Kartu/KartuKesempatan.hpp"
#include "models/Kartu/KartuDanaUmum.hpp"
#include "models/Kartu/KartuKemampuanSpesial.hpp"
#include <random>
#include <memory>

inline void isiDeckKesempatan(DeckKartu<Kartu>& deck) {
    deck.tambahKeKartu(new KartuPergiKeStasiunTerdekat());
    deck.tambahKeKartu(new KartuMundurTigaPetak());
    deck.tambahKeKartu(new KartuMasukPenjara());
    deck.tambahKeKartu(new KartuBebasPenjara());
    deck.acakUlang();
}

inline void isiDeckDanaUmum(DeckKartu<Kartu>& deck) {
    deck.tambahKeKartu(new KartuHariUlangTahun());
    deck.tambahKeKartu(new KartuBiayaDokter());
    deck.tambahKeKartu(new KartuMauNyaleg());
    deck.acakUlang();
}

inline int randRange(int lo, int hi) {
    static std::mt19937 rng{ std::random_device{}() };
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(rng);
}

inline void isiDeckSkill(DeckKartu<KartuKemampuanSpesial>& deck, bool includeBonus = true) {
    for (int i = 0; i < 4; ++i) {
        deck.tambahKeKartu(new MoveCard(randRange(1, 12)));
    }
    for (int i = 0; i < 3; ++i) {
        deck.tambahKeKartu(new DiscountCard(randRange(1, 5) * 10));
    }
    for (int i = 0; i < 2; ++i) deck.tambahKeKartu(new ShieldCard());
    for (int i = 0; i < 2; ++i) deck.tambahKeKartu(new TeleportCard());
    for (int i = 0; i < 2; ++i) deck.tambahKeKartu(new LassoCard());
    for (int i = 0; i < 2; ++i) deck.tambahKeKartu(new DemolitionCard());
    // kartu bonus
    if (includeBonus) {
        for (int i = 0; i < 2; ++i) deck.tambahKeKartu(new RotasiKartuCard());
        for (int i = 0; i < 2; ++i) deck.tambahKeKartu(new ReverseCard());
        for (int i = 0; i < 2; ++i) deck.tambahKeKartu(new PenjaraKanCard());
    }
    deck.acakUlang();
}