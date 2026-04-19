#include "utils/LogTransaksiGame.hpp"
#include <utility>
#include <algorithm>

using namespace std;

LogTransaksiEntry::LogTransaksiEntry(int ronde, string username, string aksi, string detail)
    : ronde(ronde), username(move(username)), aksi(move(aksi)), detail(move(detail)) {}

void LogTransaksiGame::tambahLog(LogTransaksiEntry log) {
    logs.push_back(move(log));
}

vector<LogTransaksiEntry> LogTransaksiGame::getLogs(int top) const {
    if (top <= 0 || logs.empty()) {
        return {};
    }

    size_t count = min(static_cast<size_t>(top), logs.size());
    return vector<LogTransaksiEntry>(logs.end() - count, logs.end());
}

LogTransaksiGame& LogTransaksiGame::operator<<(LogTransaksiEntry log) {
    tambahLog(move(log));
    return *this;
}
