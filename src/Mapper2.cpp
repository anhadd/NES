#include "../include/Mapper2.h"




Mapper2::Mapper2(uint8_t nPRG, uint8_t nCHR) {
    // Constructor
    id = 0;
    PRG_banks = nPRG;
    CHR_banks = nCHR;

    prg_bank0 = 0;
    prg_bank1 = nPRG - 1;
}


Mapper2::~Mapper2() {
    // Destructor
    
}


uint32_t Mapper2::cpuMap(uint16_t address, bool write, uint8_t value) {
    if (write) {
        prg_bank0 = value & 0x0F;
        return address;
    }
    else {
        if (address >= 0x6000 && address <= 0x7FFF) {
            return address & 0x1FFF;
        }
        else if (address >= 0x8000 && address <= 0xBFFF) {
            return prg_bank0 * PRG_BANK_SIZE + (address & 0x3FFF);
        }
        else if (address >= 0xC000 && address <= 0xFFFF) {
            return prg_bank1 * PRG_BANK_SIZE + (address & 0x3FFF);
        }
        else {
            return 0x0000; // Was: return address;
        }
    }
}


uint32_t Mapper2::ppuMap(uint16_t address, bool write, uint8_t value) {
    return address & 0x1FFF;
}
