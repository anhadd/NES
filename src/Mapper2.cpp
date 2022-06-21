#include "../include/Mapper2.h"




Mapper2::Mapper2(uint8_t nPRG, uint8_t nCHR) {
    // Constructor
    id = 2;
    PRG_banks = nPRG;
    CHR_banks = nCHR;

    prg_bank0 = 0;
    // PRG bank 1 (0xC000-0xFFFF) is always the last PRG bank.
    prg_bank1 = nPRG - 1;
}


Mapper2::~Mapper2() {
    // Destructor
}


uint32_t Mapper2::cpuMap(uint16_t address, bool write, uint8_t value) {
    if (write) {
        // Writes switch the current PRG bank.
        if (address >= 0x8000 && address <= 0xFFFF) {
            prg_bank0 = value & 0x0F;
        }
        return address;
    }
    else {
        // PRG RAM.
        if (address >= 0x6000 && address <= 0x7FFF) {
            return address & 0x1FFF;
        }
        // PRG ROM bank0.
        else if (address >= 0x8000 && address <= 0xBFFF) {
            return prg_bank0 * PRG_BANK_SIZE + (address & 0x3FFF);
        }
        // PRG ROM bank1.
        else if (address >= 0xC000 && address <= 0xFFFF) {
            return prg_bank1 * PRG_BANK_SIZE + (address & 0x3FFF);
        }
        // Any other address is not allowed.
        // This should never happen.
        else {
            fprintf(stderr, "Error: Access not allowed on Mapper2.\n");
            exit(0);
        }
    }
}


uint32_t Mapper2::ppuMap(uint16_t address, bool write, uint8_t value) {
    return address & 0x1FFF;
}
