#pragma once

class GameConstants {
public:
    // Game Rules
    static const int DENDA_PENJARA;
    static const int MAX_PERCOBAAN_PENJARA;
    
    // Starting values (can be overridden by config/input, but here are the defaults)
    static const int SALDO_AWAL_DEFAULT;

    // Logging actions
    static constexpr const char* LOG_AKSI_GERAK = "GERAK";
    static constexpr const char* LOG_AKSI_TELEPORT = "TELEPORT";
    static constexpr const char* LOG_AKSI_PENJARA = "PENJARA";
    static constexpr const char* LOG_AKSI_KARTU = "KARTU";
    static constexpr const char* LOG_AKSI_FESTIVAL = "FESTIVAL";
};
