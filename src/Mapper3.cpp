#include "../include/Mapper3.h"




Mapper3::Mapper3(uint8_t nPRG, uint8_t nCHR) {
    // Constructor
    id = 0;
    PRG_banks = nPRG;
    CHR_banks = nCHR;
    
    chr_bank0 = 0;
    chr_bank1 = 0;
}


Mapper3::~Mapper3() {
    // Destructor
    
}


uint32_t Mapper3::cpuMap(uint16_t address, bool write, uint8_t value) {
    if (write) {
        chr_bank0 = value & 0x03;
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


uint32_t Mapper3::ppuMap(uint16_t address, bool write, uint8_t value) {
    return (chr_bank0 * 0x2000) + (address & 0x1FFF);
}
