#include "../include/Mapper0.h"



Mapper0::Mapper0(uint8_t nPRG, uint8_t nCHR) {
    // Constructor
    id = 0;
    PRG_banks = nPRG;
    CHR_banks = nCHR;
}


Mapper0::~Mapper0() {
    // Destructor
    
}

uint32_t Mapper0::cpuMap(uint16_t address, bool write, uint8_t value) {
    // The address refers to the PRG RAM.
    if (address >= 0x6000 && address <= 0x7FFF) {
        return address & 0x1FFF;
    }
    // The address refers to the PRG ROM.
    else if (address >= 0x8000 && address <= 0xFFFF) {
        if (PRG_banks == 1) {
            return address & 0x3FFF;
        }
        else {
            return address & 0x7FFF;
        }
    }
    // Any other address is not allowed.
    // This should never happen.
    else {
        fprintf(stderr, "Error: Access not allowed on Mapper0.\n");
        exit(0);
    }
}

uint32_t Mapper0::ppuMap(uint16_t address, bool write, uint8_t value) {
    return address & 0x1FFF;
}
