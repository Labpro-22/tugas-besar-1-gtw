#include "models/Kartu/DeckFactory.hpp"
#include "models/Kartu/DeckKartu.hpp"
#include "models/Kartu/Kartu.hpp"
#include "models/Kartu/KartuDanaUmum.hpp"
#include "models/Kartu/KartuKesempatan.hpp"
#include "models/Kartu/KartuKemampuanSpesial.hpp"
#include "models/Pemain.hpp"
#include "models/Petak/PetakFestival.hpp"
#include "models/Petak/PetakKartu.hpp"
#include "models/PlayerActionService.hpp"
#include "models/Properti/Properti.hpp"
#include "utils/LogTransaksiGame.hpp"
#include <cassert>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>

using namespace std;

static void printSep(const std::string& judul) {
    std::cout << "  " << judul << "\n";
}

class PropertiDummy : public Properti {
public:
    PropertiDummy(const PropertiConfig& config) : Properti(config) {}
    int hitungSewa(int, ManagerProperti&) override {
        return 100;
    }
};

static PlayerActionService makeSvc(std::vector<Pemain*>* daftar, LogTransaksiGame* log, int* turnIdx) {
    return PlayerActionService(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, log, daftar, turnIdx);
}

void testBuild() {
    printSep("TEST 1: Build / Smoke Test");
    std::cout << "[PASS] Binary berhasil dijalankan.\n";
}

void testDeckKartu() {
    printSep("TEST 2: DeckKartu<KartuKemampuanSpesial>");
    DeckKartu<KartuKemampuanSpesial> deck;
    isiDeckSkill(deck, false);

    std::cout << "Jumlah kartu dalam deck: " << deck.ukuran() << " (expected: 15)\n";
    assert(deck.ukuran() == 15);

    int diambil = 0;
    while (deck.ukuran() > 0) {
        KartuKemampuanSpesial* k = deck.ambilKartu();
        std::cout << "  Ambil: " << k->getDeskripsi() << "\n";
        deck.buangKartu(k);
        diambil++;
    }

    std::cout << "Total diambil: " << diambil << " (expected: 15)\n";
    std::cout << "Ukuran deck setelah dibuang semua: " << deck.ukuran() << " (expected: 0)\n";
    std::cout << "Ukuran pembuangan: " << deck.ukuranPembuangan() << " (expected: 15)\n";

    assert(diambil == 15);
    assert(deck.ukuran() == 0);
    assert(deck.ukuranPembuangan() == 15);

    KartuKemampuanSpesial* lagi = deck.ambilKartu();
    std::cout << "Setelah acakUlang, ambil lagi: " << lagi->getDeskripsi() << "\n";
    assert(lagi != nullptr);
    deck.buangKartu(lagi);

    std::cout << "[PASS] DeckKartu generic OK\n";
}

void testPetakKartu() {
    printSep("TEST 3: PetakKartu");

    DeckKartu<Kartu> deckDana;
    deckDana.tambahKeKartu(new KartuBiayaDokter());

    Pemain p("Bob", 1500);
    std::vector<Pemain*> daftar = { &p };
    int turnIdx = 0;
    LogTransaksiGame log;
    PlayerActionService svc = makeSvc(&daftar, &log, &turnIdx);

    PetakKartu petak(18, "DNU", "Dana Umum", PetakKartu::JenisKartu::DANA_UMUM, &deckDana);

    int saldoAwal = p.getSaldo();
    petak.onLanded(p, svc);

    std::cout << "Saldo setelah BiayaDokter: " << p.getSaldo() << " (expected: " << saldoAwal - 700 << ")\n";
    std::cout << "Deck utama: " << deckDana.ukuran() << " (expected: 0)\n";
    std::cout << "Pembuangan: " << deckDana.ukuranPembuangan() << " (expected: 1)\n";
    std::cout << "Jumlah log: " << log.getLogs().size() << " (expected: >=1)\n";

    assert(p.getSaldo() == saldoAwal - 700);
    assert(deckDana.ukuran() == 0);
    assert(deckDana.ukuranPembuangan() == 1);
    assert(!log.getLogs().empty());
    assert(log.getLogs().back().getAksi() == "KARTU");

    std::cout << "[PASS] PetakKartu OK\n";
}

void testKartuDanaUmum() {
    printSep("TEST 4: Kartu Dana Umum");

    Pemain a("Alice", 1000);
    Pemain b("Bob", 1000);
    Pemain c("Charlie", 1000);

    std::vector<Pemain*> daftar = { &a, &b, &c };
    int turnIdx = 0;
    LogTransaksiGame log;
    PlayerActionService svc = makeSvc(&daftar, &log, &turnIdx);

    {
        KartuHariUlangTahun k;
        k.jalankanEfek(a, svc);
        std::cout << "Setelah HariUlangTahun -> Alice: " << a.getSaldo() << ", Bob: " << b.getSaldo() << ", Charlie: " << c.getSaldo() << "\n";
        assert(a.getSaldo() == 1200);
        assert(b.getSaldo() == 900);
        assert(c.getSaldo() == 900);
    }

    a = Pemain("Alice", 1000);
    b = Pemain("Bob", 1000);
    c = Pemain("Charlie", 1000);
    daftar = { &a, &b, &c };

    {
        KartuBiayaDokter k;
        k.jalankanEfek(a, svc);
        std::cout << "Setelah BiayaDokter -> Alice: " << a.getSaldo() << "\n";
        assert(a.getSaldo() == 300);
        assert(b.getSaldo() == 1000);
        assert(c.getSaldo() == 1000);
    }

    a = Pemain("Alice", 1000);
    b = Pemain("Bob", 1000);
    c = Pemain("Charlie", 1000);
    daftar = { &a, &b, &c };

    {
        KartuMauNyaleg k;
        k.jalankanEfek(a, svc);
        std::cout << "Setelah MauNyaleg -> Alice: " << a.getSaldo() << ", Bob: " << b.getSaldo() << ", Charlie: " << c.getSaldo() << "\n";
        assert(a.getSaldo() == 600);
        assert(b.getSaldo() == 1200);
        assert(c.getSaldo() == 1200);
    }

    std::cout << "[PASS] Kartu Dana Umum OK\n";
}

void testKartuKemampuanWajib() {
    printSep("TEST 5: Kartu Kemampuan Spesial");

    Pemain a("Alice", 1000);
    Pemain b("Bob", 1000);
    Pemain c("Charlie", 1000);

    a.setPosisi(5);
    b.setPosisi(8);
    c.setPosisi(20);

    std::vector<Pemain*> daftar = { &a, &b, &c };
    int turnIdx = 0;
    LogTransaksiGame log;
    PlayerActionService svc = makeSvc(&daftar, &log, &turnIdx);

    {
        MoveCard k(4);
        k.gunakan(a, svc);
        assert(a.getPosisi() == 9);
    }

    {
        DiscountCard k(30);
        k.gunakan(a, svc);
        assert(a.isDiscountAktif());
        assert(a.getPersenDiskon() == 30);
    }

    {
        ShieldCard k;
        k.gunakan(a, svc);
        assert(a.isShieldAktif());
    }

    {
        std::streambuf* oldCin = std::cin.rdbuf();
        std::istringstream fakeInput("GO\n");
        std::cin.rdbuf(fakeInput.rdbuf());

        size_t logSebelum = log.getLogs().size();
        TeleportCard k;
        k.gunakan(a, svc);

        std::cin.rdbuf(oldCin);

        assert(log.getLogs().size() == logSebelum + 1);
        assert(log.getLogs().back().getAksi() == "TELEPORT");
    }

    {
        a.setPosisi(10);
        b.setPosisi(13);
        c.setPosisi(25);

        LassoCard k;
        k.gunakan(a, svc);
        assert(b.getPosisi() == 10);
    }

    {
        size_t logSebelum = log.getLogs().size();
        DemolitionCard k;
        k.gunakan(a, svc);

        assert(log.getLogs().size() == logSebelum + 1);
    }

    std::cout << "[PASS] KKS OK\n";
}

void testSlotPenuh() {
    printSep("TEST 6: Slot penuh");

    DeckKartu<KartuKemampuanSpesial> deckSkill;
    Pemain p("Alice", 2000);

    p.tambahKartu(new MoveCard(3), deckSkill);
    p.tambahKartu(new ShieldCard(), deckSkill);
    p.tambahKartu(new LassoCard(), deckSkill);

    std::streambuf* oldCin = std::cin.rdbuf();
    std::istringstream fakeInput("1\n");
    std::cin.rdbuf(fakeInput.rdbuf());

    p.tambahKartu(new TeleportCard(), deckSkill);

    std::cin.rdbuf(oldCin);

    assert(p.getKartuDiTangan().size() == 3);
    assert(deckSkill.ukuranPembuangan() == 1);

    std::cout << "[PASS] Slot penuh OK\n";
}

void testOneSkillPerTurn() {
    printSep("TEST 7: One skill per turn");

    DeckKartu<KartuKemampuanSpesial> deckSkill;
    Pemain p("Dave", 1000);
    std::vector<Pemain*> daftar = { &p };
    int turnIdx = 0;
    LogTransaksiGame log;
    PlayerActionService svc = makeSvc(&daftar, &log, &turnIdx);

    p.tambahKartu(new ShieldCard(), deckSkill);
    p.tambahKartu(new MoveCard(4), deckSkill);

    p.gunakanKartu(0, svc, deckSkill, true);
    size_t sisa = p.getKartuDiTangan().size();

    p.gunakanKartu(0, svc, deckSkill, true);
    assert(p.getKartuDiTangan().size() == sisa);

    p.resetSkillUsage();
    p.gunakanKartu(0, svc, deckSkill, false);
    assert(p.getKartuDiTangan().size() == sisa);

    std::cout << "[PASS] One skill OK\n";
}

void testFestival() {
    printSep("TEST 8: Festival");
    PropertiConfig cfg(
        999,
        "TST",
        "TestProperti",
        "STREET",
        "HIJAU",
        100,
        50,
        50,
        100,
        std::vector<int>{10, 20, 30, 40, 50, 60}
    );

    PropertiDummy prop(cfg);
    Pemain owner("Owner", 1000);
    prop.setPemilik(&owner);

    ManagerFestival mgr;

    mgr.applyFestival(&prop);
    mgr.applyFestival(&prop);
    mgr.applyFestival(&prop);

    assert(mgr.getPengali(&prop) == 8);

    mgr.updateDurasi(&owner);
    mgr.updateDurasi(&owner);
    mgr.updateDurasi(&owner);

    assert(!mgr.isAktif(&prop));

    std::cout << "[PASS] Festival OK\n";
}

void testDeckFactory() {
    printSep("TEST 9: DeckFactory");

    DeckKartu<Kartu> deckK, deckD;
    isiDeckKesempatan(deckK);
    isiDeckDanaUmum(deckD);

    assert(deckK.ukuran() == 4);
    assert(deckD.ukuran() == 3);

    DeckKartu<KartuKemampuanSpesial> deckSkill;
    isiDeckSkill(deckSkill, true);

    assert(deckSkill.ukuran() == 21);

    std::cout << "[PASS] DeckFactory OK\n";
}

void testBonusReverseCard() {
    printSep("TEST 10: Reverse");

    Pemain p("Alice", 1000);
    std::vector<Pemain*> daftar = { &p };
    int turnIdx = 0;
    LogTransaksiGame log;
    PlayerActionService svc = makeSvc(&daftar, &log, &turnIdx);

    ReverseCard k;
    k.gunakan(p, svc);
    k.gunakan(p, svc);

    std::cout << "[PASS] Reverse OK\n";
}

void testBonusRotasiKartuCard() {
    printSep("TEST 11: Rotasi");

    Pemain p1("P1", 1000);
    Pemain p2("P2", 1000);
    Pemain p3("P3", 1000);

    DeckKartu<KartuKemampuanSpesial> deck;
    p1.tambahKartu(new MoveCard(1), deck);
    p2.tambahKartu(new ShieldCard(), deck);
    p3.tambahKartu(new DiscountCard(20), deck);

    std::vector<Pemain*> daftar = { &p1, &p2, &p3 };
    int turnIdx = 0;
    LogTransaksiGame log;
    PlayerActionService svc = makeSvc(&daftar, &log, &turnIdx);

    RotasiKartuCard k;
    k.gunakan(p1, svc);

    std::cout << "[PASS] Rotasi OK\n";
}

void testBonusPenjaraKanCard() {
    printSep("TEST 12: PenjaraKan");

    Pemain p1("Alice", 1000);
    Pemain p2("Bob", 1000);
    Pemain p3("Charlie", 1000);

    std::vector<Pemain*> daftar = { &p1, &p2, &p3 };
    int turnIdx = 0;
    LogTransaksiGame log;
    PlayerActionService svc = makeSvc(&daftar, &log, &turnIdx);

    std::streambuf* oldCin = std::cin.rdbuf();
    std::istringstream fakeInput("1\n");
    std::cin.rdbuf(fakeInput.rdbuf());

    PenjaraKanCard k;
    k.gunakan(p1, svc);

    std::cin.rdbuf(oldCin);

    std::cout << "[PASS] PenjaraKan OK\n";
}

int main() {
    std::cout << "=== testing branch kartu ===\n";

    testBuild();
    testDeckKartu();
    testPetakKartu();
    testKartuDanaUmum();
    testKartuKemampuanWajib();
    testSlotPenuh();
    testOneSkillPerTurn();
    testFestival();
    testDeckFactory();
    testBonusReverseCard();
    testBonusRotasiKartuCard();
    testBonusPenjaraKanCard();

    std::cout << "\n=== semua test selesai ===\n";
    return 0;
}