#include "../include/BUS.h"



BUS::BUS() {
    // Constructor
    fill(begin(memory), end(memory), 0);
}


BUS::~BUS() {
    // Destructor
    
}

void BUS::passPPU(PPU* nesPPU) {
    ppu = nesPPU;
}

uint8_t BUS::busReadCPU(uint16_t address) {
    if (address <= 0x1FFF) {
        return memory[address & 0x07FF];
    }
    else if (address <= 0x3FFF) {
        return ppu->readRegister(address);
    }
    else {
        return memory[address];
    }
    return 0x00;
}

uint8_t BUS::busWriteCPU(uint16_t address, uint8_t value) {
    if (address <= 0x1FFF) {
        memory[address & 0x07FF] = value;
    }
    else if (address <= 0x3FFF) {
        ppu->writeRegister(address, value);
    }
    else {
        memory[address] = value;
    }
    return 0;
}

uint8_t BUS::busReadPPU(uint16_t address) {
    return ppu->ppuRead(address);
}

uint8_t BUS::busWritePPU(uint16_t address, uint8_t value) {
    ppu->ppuWrite(address, value);
    return 0;
}
