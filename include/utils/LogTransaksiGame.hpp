#pragma once
#include <vector>
#include <string>

class LogTransaksiEntry{
private:
    int ronde;
    std::string username;
    std::string aksi; 
    std::string detail; 
public:
    LogTransaksiEntry(int ronde, std::string username, std::string aksi, std::string detail);
    int getRonde() const { return ronde; }
    const std::string& getUsername() const { return username; }
    const std::string& getAksi() const { return aksi; }
    const std::string& getDetail() const { return detail; }

};

class LogTransaksiGame{
private:
    std::vector<LogTransaksiEntry> logs;
public:
    void tambahLog(LogTransaksiEntry log);
    const std::vector<LogTransaksiEntry>& getLogs() const { return logs; }
    std::vector<LogTransaksiEntry> getLogs(int top) const;
    LogTransaksiGame& operator<<(LogTransaksiEntry log);
};