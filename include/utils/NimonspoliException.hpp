#pragma once
#include <exception>
#include <string>

class NimonspoliException : public std::exception {
protected:
    int kodeError;
    std::string tipeError;
public:
    /**
     * @brief Constructor dari base NimonspoliException.
     * @param kode Kode representasi numerik dari error.
     * @param tipe Nama/tipe kategori dari error.
     */
    NimonspoliException(int kode, std::string tipe) : kodeError(kode), tipeError(tipe) {}

    int getkodeError() const noexcept { return kodeError; }
    std::string gettipeError() const noexcept { return tipeError; }
    virtual const char* what() const noexcept override { return tipeError.c_str(); }
};

class UangTidakCukupException : public NimonspoliException{
public:
    UangTidakCukupException() : NimonspoliException(1, "UANG_TIDAK_CUKUP"){};
};

class SlotKartuPenuhException : public NimonspoliException{
public:
    SlotKartuPenuhException() : NimonspoliException(2, "SLOT_KARTU_PENUH"){};
};

// Untuk handle command yang tidak ada
class PerintahTidakDitemukanException : public NimonspoliException{
public:
    PerintahTidakDitemukanException() : NimonspoliException(3, "PERINTAH_TIDAK_DITEMUKAN"){};
};

// Untuk handle path file yang tidak ada atau file config yang kosong
class FileTidakValidException : public NimonspoliException{
public:
    FileTidakValidException() : NimonspoliException(4, "FILE_TIDAK_VALID"){};
};

