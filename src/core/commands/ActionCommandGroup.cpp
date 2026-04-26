#include "core/commands/ActionCommandGroup.hpp"

#include "core/commands/TurnCommandContext.hpp"
#include "models/Kartu/DeckKartu.hpp"
#include "models/Kartu/KartuKemampuanSpesial.hpp"
#include "models/Managers/ManagerPenjara.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "utils/SaveNLoad.hpp"
#include "views/InputHandler.hpp"
#include "views/OutputHandler.hpp"

bool ActionCommandGroup::canHandle(const std::string& cmd) const {
    return cmd == "GADAI" || cmd == "TEBUS" || cmd == "BANGUN" || cmd == "GUNAKAN_KEMAMPUAN" || cmd == "BAYAR_DENDA" || cmd == "SIMPAN";
}

bool ActionCommandGroup::execute(const std::string& cmd, const std::vector<std::string>& tok, TurnCommandContext& ctx) {
    if (cmd == "GADAI") {
        if (ctx.state.sudahLemparDadu) {
            OutputHandler::cetakError("GADAI hanya bisa dilakukan SEBELUM melempar dadu.");
            return false;
        }
        if (ctx.configData) ctx.actionService->gadaiProperti(*ctx.pemain);
        ctx.state.awalGiliran = false;
        ctx.state.adaAksiStateChange = true;
        return false;
    }

    if (cmd == "TEBUS") {
        if (ctx.configData) ctx.actionService->tebusProperti(*ctx.pemain);
        ctx.state.awalGiliran = false;
        ctx.state.adaAksiStateChange = true;
        return false;
    }

    if (cmd == "BANGUN") {
        if (ctx.configData) ctx.actionService->bangunProperti(*ctx.pemain);
        ctx.state.awalGiliran = false;
        ctx.state.adaAksiStateChange = true;
        return false;
    }

    if (cmd == "GUNAKAN_KEMAMPUAN") {
        if (ctx.state.sudahLemparDadu) {
            OutputHandler::cetakError("Kartu kemampuan hanya bisa digunakan SEBELUM melempar dadu.");
            return false;
        }
        const auto& hand = ctx.pemain->getKartuDiTangan();
        if (hand.empty()) {
            OutputHandler::cetakError("Kamu tidak punya kartu kemampuan.");
            return false;
        }
        OutputHandler::cetakPesan("Daftar Kartu Kemampuan Spesial Anda:");
        for (size_t i = 0; i < hand.size(); ++i) {
            OutputHandler::cetakPesan(std::to_string(i + 1) + ". " + hand[i]->getDeskripsi());
        }
        int pilih = InputHandler::promptAngka("Pilih kartu (1-" + std::to_string(hand.size()) + ", 0 batal): ", 0, hand.size());
        if (pilih == 0) return false;
        ctx.pemain->gunakanKartu(pilih - 1, *ctx.actionService, *ctx.deckSkill, true);
        ctx.state.awalGiliran = false;
        ctx.state.adaAksiStateChange = true;
        return false;
    }

    if (cmd == "BAYAR_DENDA") {
        if (ctx.pemain->getStatus() == StatusPemain::JAILED) {
            if (ctx.configData && ctx.pemain->getSaldo() >= ctx.configData->getDendaPenjara()) {
                ctx.managerPenjara->escapeByFine(*ctx.pemain, *ctx.actionService);
                ctx.state.adaAksiStateChange = true;
            } else {
                OutputHandler::cetakError("Uang tidak cukup untuk bayar denda penjara.");
            }
        } else {
            OutputHandler::cetakError("Kamu tidak sedang dipenjara.");
        }
        return false;
    }

    if (!ctx.state.awalGiliran || ctx.state.sudahLemparDadu || ctx.state.adaAksiStateChange) {
        OutputHandler::cetakError("Tidak bisa menyimpan game di tengah-tengah giliran!");
        return false;
    }
    if (tok.size() < 2) {
        OutputHandler::cetakError("Format: SIMPAN <filename>");
        return false;
    }
    SaveNLoad snl;
    snl.saveGameState(ctx.engine, tok[1]);
    OutputHandler::cetakPesan("Permainan berhasil disimpan ke: " + tok[1]);
    return false;
}

