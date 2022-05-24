#include "../include/Mapper3.h"




Mapper3::Mapper3() {
    // Constructor
    id = 2;
    PRG_banks = 0;
    CHR_banks = 0;

    selected_bank1 = 0;
    selected_bank2 = 0;
}


Mapper3::~Mapper3() {
    // Destructor
    
}

uint32_t Mapper3::cpuMap(uint16_t address, bool write, uint8_t value) {
    if (write) {
        selected_bank1 = value;
        return address;
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

uint32_t Mapper3::ppuMap(uint16_t address) {
    return (selected_bank1 * 0x2000) + (address & 0x1FFF);
}
