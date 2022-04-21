#include "../include/PPU.h"



PPU::PPU() {
    // Constructor
    
}


PPU::~PPU() {
    // Destructor
    
}

void PPU::passBUS(BUS* nesBUS) {
    bus = nesBUS;
}

uint8_t PPU::ppuRead(uint16_t address) {
    return bus->busRead(address);
}

uint8_t PPU::ppuWrite(uint16_t address, uint8_t value) {
    bus->busWrite(address, value);
    return 0;
}

bool PPU::executeCycle() {
    // Execute a single cycle.
    // TODO: IMPLEMENT THIS !!!
    
    return 0;
}
