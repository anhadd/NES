#include "../include/Mapper1.h"



Mapper1::Mapper1() {
    // Constructor
    id = 1;
    PRG_banks = 0;
    CHR_banks = 0;
}


Mapper1::~Mapper1() {
    // Destructor
    
}

uint16_t Mapper1::cpuMap(uint16_t address) {
    if (PRG_banks == 1) {
        return address & 0x3FFF;
    }
    else {
        return address & 0x7FFF;
    }
}

uint16_t Mapper1::ppuMap(uint16_t address) {
    return address & 0x1FFF;
}
