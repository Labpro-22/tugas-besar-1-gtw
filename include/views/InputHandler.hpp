#pragma once

#include <string>
#include <iostream>
#include <limits>

class InputHandler {
public:
    // UI Helpers untuk meminta input berulang kali hingga valid
    static int promptAngka(const std::string& pesan, int min, int max);
    static std::string promptString(const std::string& pesan);
    
    // Spesifik prompt untuk pajak
    static int mintaPilihanPajak(int pphFlat, int pphPersen);
};
