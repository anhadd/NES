#include "../include/BUS.h"



BUS::BUS() {
    // Constructor

}


BUS::~BUS() {
    // Destructor
    
}

void BUS::passPPU(PPU* nesPPU) {
    ppu = nesPPU;
}

void BUS::passCPU(CPU* nesCPU) {
    cpu = nesCPU;
}

uint8_t BUS::busReadCPU(uint16_t address) {
    if (address >= 0x2000 && address <= 0x3FFF) {
        return ppu->readRegister(address);
    }
    else {
        return cpu->cpuRead(address);
    }
    return 0x00;
}

uint8_t BUS::busWriteCPU(uint16_t address, uint8_t value) {
    if (address >= 0x2000 && address <= 0x3FFF) {
        ppu->writeRegister(address, value);
    }
    else {
        cpu->cpuWrite(address, value);
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
