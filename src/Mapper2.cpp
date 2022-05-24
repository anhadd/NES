#include "../include/Mapper2.h"




Mapper2::Mapper2() {
    // Constructor
    id = 2;
    PRG_banks = 0;
    CHR_banks = 0;

    selected_bank1 = 0;
    selected_bank2 = 0;
}


Mapper2::~Mapper2() {
    // Destructor
    
}

uint32_t Mapper2::cpuMap(uint16_t address, bool write, uint8_t value) {
    if (write) {
        selected_bank1 = value & 0x0F;
        return address;
    }
    else {
        if (address >= 0x6000 && address <= 0x7FFF) {
            return address & 0x1FFF;
        }
        else if (address >= 0x8000 && address <= 0xBFFF) {
            return selected_bank1 * PRG_BANK_SIZE + (address & 0x3FFF);
        }
        else if (address >= 0xC000 && address <= 0xFFFF) {
            return selected_bank2 * PRG_BANK_SIZE + (address & 0x3FFF);
        }
        else {
            return 0x0000; // Was: return address;
        }
    }
}

uint32_t Mapper2::ppuMap(uint16_t address) {
    return address & 0x1FFF;
}
