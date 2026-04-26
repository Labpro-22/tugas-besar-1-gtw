#include "core/commands/DiceCommandGroup.hpp"

#include "core/commands/TurnCommandContext.hpp"
#include "models/Dadu.hpp"
#include "models/Pemain.hpp"
#include "models/PlayerActionService.hpp"
#include "views/OutputHandler.hpp"

bool DiceCommandGroup::canHandle(const std::string& cmd) const {
    return cmd == "LEMPAR_DADU" || cmd == "ATUR_DADU";
}

bool DiceCommandGroup::execute(const std::string& cmd, const std::vector<std::string>& tok, TurnCommandContext& ctx) {
    if (ctx.state.sudahLemparDadu) {
        OutputHandler::cetakError("Kamu sudah melempar dadu di giliran ini.");
        return false;
    }

    if (cmd == "LEMPAR_DADU") {
        ctx.dadu->rollRandom();
        OutputHandler::cetakPesan("Hasil: " + std::to_string(ctx.dadu->getNilaid1()) + " + " + std::to_string(ctx.dadu->getNilaid2()) + " = " + std::to_string(ctx.dadu->getTotalNilaiDadu()));
    } else {
        if (tok.size() < 3) {
            OutputHandler::cetakError("Format: ATUR_DADU X Y");
            return false;
        }
        int x = 0;
        int y = 0;
        try {
            x = std::stoi(tok[1]);
            y = std::stoi(tok[2]);
        } catch (...) {
            OutputHandler::cetakError("Nilai dadu harus berupa angka 1-6.");
            return false;
        }
        if (x < 1 || x > 6 || y < 1 || y > 6) {
            OutputHandler::cetakError("Nilai dadu harus 1-6.");
            return false;
        }
        ctx.dadu->rollManual(x, y);
        OutputHandler::cetakPesan("Hasil: " + std::to_string(x) + " + " + std::to_string(y) + " = " + std::to_string(ctx.dadu->getTotalNilaiDadu()));
    }

    bool isDouble = ctx.dadu->isDouble();
    if (ctx.pemain->getStatus() == StatusPemain::JAILED) {
        if (isDouble) {
            OutputHandler::cetakPesan("Double! Kamu berhasil keluar dari penjara.");
            ctx.pemain->setStatus(StatusPemain::ACTIVE);
            ctx.pemain->resetPercobaanPenjara();
            ctx.actionService->movePlayerRelative(*ctx.pemain, ctx.dadu->getTotalNilaiDadu());
            ctx.state.extraTurn = false;
        } else {
            OutputHandler::cetakPesan("Bukan double. Kamu tetap di penjara.");
            ctx.pemain->tambahPercobaanPenjara();
            ctx.state.extraTurn = false;
        }
        ctx.state.sudahLemparDadu = true;
        return true;
    }

    if (isDouble) {
        if (ctx.state.doubleCount) {
            (*ctx.state.doubleCount)++;
            OutputHandler::cetakPesan("[DOUBLE] " + std::to_string(*ctx.state.doubleCount) + "x berturut-turut!");
            if (*ctx.state.doubleCount == 3) {
                OutputHandler::cetakPesan("3x double! Kamu melanggar batas kecepatan dan masuk penjara!");
                ctx.actionService->sendToJail(*ctx.pemain);
                ctx.state.extraTurn = false;
                ctx.state.sudahLemparDadu = true;
                return true;
            }
        }
        ctx.state.extraTurn = true;
    } else {
        ctx.state.extraTurn = false;
    }

    ctx.actionService->movePlayerRelative(*ctx.pemain, ctx.dadu->getTotalNilaiDadu());
    ctx.state.sudahLemparDadu = true;
    ctx.state.adaAksiStateChange = true;
    return true;
}

