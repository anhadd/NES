#include "../include/Mapper1.h"



// TODO: CONTINUE MAPPER 1, NOT FINISHED YET!

Mapper1::Mapper1() {
    // Constructor
    id = 1;
    PRG_banks = 0;
    CHR_banks = 0;
}


Mapper1::~Mapper1() {
    // Destructor
    
}

uint32_t Mapper1::cpuMap(uint16_t address, bool write, uint8_t value) {
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

uint32_t Mapper1::ppuMap(uint16_t address) {
    return address & 0x1FFF;
}
