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

uint16_t Mapper0::cpuMap(uint16_t address) {
    // TODO: FIX WHY THE BANK NUMBERS ARE WRONG HERE !
    fprintf(stderr, "PRG BANKS MAPPING: %u\n", PRG_banks);
    fprintf(stderr, "CHR BANKS MAPPING: %u\n", CHR_banks);
    if (PRG_banks == 1) {
        return address & 0x3FFF;
    }
    else {
        return address & 0x7FFF;
    }
}

uint16_t Mapper0::ppuMap(uint16_t address) {
    return address & 0x1FFF;
}
