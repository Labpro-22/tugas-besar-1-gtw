#include "models/Petak/PetakFestival.hpp"
#include "models/Managers/ManagerFestival.hpp"
#include "models/Petak/PetakProperti.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "views/InputHandler.hpp"
#include "views/OutputHandler.hpp"



PetakFestival::PetakFestival(int indeks, const std::string& kode, const std::string& nama, ManagerFestival* mgr) : PetakAksi(indeks, kode, nama), managerFestival(mgr) {}

void PetakFestival::onLanded(Pemain& pemain, PlayerActionService& svc) {
    std::cout << "\nKamu mendarat di petak Festival!\n";
    const auto& aset = pemain.getAsetPemain();
    if (aset.empty()) {
        std::cout << "Kamu tidak memiliki properti.\n";
        return;
    }
    std::cout << "Daftar properti milikmu:\n";
    for (size_t i = 0; i < aset.size(); ++i) {
        PetakProperti* p = aset[i];
        std::cout << (i + 1) << ". " << p->getNama() << " (" << p->getKode() << ")";
        if (managerFestival->isAktif(p)) {
            std::cout << " [festival x" << managerFestival->getPengali(p) << ", sisa " << managerFestival->getSisaDurasi(p) << " giliran]";
        }
        std::cout << "\n";
    }
    int pilihan = InputHandler::promptAngka("Pilih properti (1-" + std::to_string(aset.size()) + "): ", 1, aset.size());
    PetakProperti* target = aset[pilihan - 1];
    managerFestival->applyFestival(target);
    std::string detail = target->getKode() + " festival x" + std::to_string(managerFestival->getPengali(target)) + " durasi " + std::to_string(managerFestival->getSisaDurasi(target));
    svc.logAksi(pemain, "FESTIVAL", detail);
}