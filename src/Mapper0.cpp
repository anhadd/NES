#include "../include/Mapper0.h"



Mapper0::Mapper0() {
    // Constructor
    id = 0;
    PRG_banks = 0;
    CHR_banks = 0;
}


Mapper0::~Mapper0() {
    // Destructor
    
}

uint32_t Mapper0::cpuMap(uint16_t address, bool write, uint8_t value) {
    if (address >= 0x6000 && address <= 0x7FFF) {
        return address & 0x1FFF;
    }
    else {
        if (PRG_banks == 1) {
            return address & 0x3FFF;
        }
        else {
            return address & 0x7FFF;
        }
    }
}

uint32_t Mapper0::ppuMap(uint16_t address) {
    return address & 0x1FFF;
}
