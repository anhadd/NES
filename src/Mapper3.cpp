#include "../include/Mapper3.h"




Mapper3::Mapper3(uint8_t nPRG, uint8_t nCHR) {
    // Constructor
    id = 3;
    PRG_banks = nPRG;
    CHR_banks = nCHR;
    
    chr_bank0 = 0;
}


Mapper3::~Mapper3() {
    // Destructor
    
}


uint32_t Mapper3::cpuMap(uint16_t address, bool write, uint8_t value) {
    // Writes switch the current CHR bank.
    if (write) {
        chr_bank0 = value & 0x03;
        return address;
    }
    else {
        // PRG RAM.
        if (address >= 0x6000 && address <= 0x7FFF) {
            return address & 0x1FFF;
        }
        // PRG ROM.
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
            fprintf(stderr, "Error: Access not allowed on Mapper2.\n");
            exit(0);
        }
    }
}


uint32_t Mapper3::ppuMap(uint16_t address, bool write, uint8_t value) {
    // Return the absolute address in the cartridge CHR memory.
    return (chr_bank0 * 0x2000) + (address & 0x1FFF);
}
