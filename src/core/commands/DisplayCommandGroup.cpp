#include "core/commands/DisplayCommandGroup.hpp"

#include "core/commands/TurnCommandContext.hpp"
#include "models/Papan.hpp"
#include "models/Pemain.hpp"
#include "models/Petak/PetakProperti.hpp"
#include "utils/LogTransaksiGame.hpp"
#include "views/InputHandler.hpp"
#include "views/OutputHandler.hpp"

bool DisplayCommandGroup::canHandle(const std::string& cmd) const {
    return cmd == "CETAK_PAPAN" || cmd == "CETAK_AKTA" || cmd == "CETAK_PROPERTI" || cmd == "CETAK_LOG" || cmd == "BANTUAN" || cmd == "HELP";
}

bool DisplayCommandGroup::execute(const std::string& cmd, const std::vector<std::string>& tok, TurnCommandContext& ctx) {
    if (cmd == "CETAK_PAPAN") {
        OutputHandler::cetakPapan(*ctx.papanPermainan, ctx.listPemain, ctx.currentTurn, ctx.maxTurn);
        return false;
    }

    if (cmd == "CETAK_AKTA") {
        if (tok.size() >= 2) {
            Petak* petak = ctx.papanPermainan->getPetak(tok[1]);
            auto prop = dynamic_cast<PetakProperti*>(petak);
            if (!prop) OutputHandler::cetakError("Kode properti tidak valid!");
            else if (ctx.configData) OutputHandler::cetakAktaProperti(prop, *ctx.configData);
        } else {
            std::string kode = InputHandler::promptString("Masukkan kode petak: ");
            Petak* petak = ctx.papanPermainan->getPetak(kode);
            auto prop = dynamic_cast<PetakProperti*>(petak);
            if (!prop) OutputHandler::cetakError("Kode properti tidak valid!");
            else if (ctx.configData) OutputHandler::cetakAktaProperti(prop, *ctx.configData);
        }
        return false;
    }

    if (cmd == "CETAK_PROPERTI") {
        if (ctx.configData) OutputHandler::cetakPropertiPemain(ctx.pemain, *ctx.configData);
        return false;
    }

    if (cmd == "CETAK_LOG") {
        int top = -1;
        if (tok.size() >= 2) {
            try { top = std::stoi(tok[1]); }
            catch (...) {
                OutputHandler::cetakError("Argumen CETAK_LOG harus berupa angka.");
                return false;
            }
        }
        if (ctx.logGame) {
            if (top > 0) {
                auto logs = ctx.logGame->getLogs(top);
                OutputHandler::cetakPesan("=== Log Transaksi (" + std::to_string(top) + " Terakhir) ===");
                for (const auto& e : logs) {
                    OutputHandler::cetakPesan("[Turn " + std::to_string(e.getRonde()) + "] " + e.getUsername() + " | " + e.getAksi() + " | " + e.getDetail());
                }
            } else {
                OutputHandler::cetakPesan("=== Log Transaksi Penuh ===");
                for (const auto& e : ctx.logGame->getLogs()) {
                    OutputHandler::cetakPesan("[Turn " + std::to_string(e.getRonde()) + "] " + e.getUsername() + " | " + e.getAksi() + " | " + e.getDetail());
                }
            }
        }
        return false;
    }

    OutputHandler::cetakPesan("Perintah tersedia: CETAK_PAPAN, CETAK_PROPERTI, CETAK_AKTA, CETAK_LOG [N], GADAI, TEBUS, BANGUN, GUNAKAN_KEMAMPUAN, LEMPAR_DADU, ATUR_DADU X Y, BAYAR_DENDA, SIMPAN <file>");
    return false;
}

