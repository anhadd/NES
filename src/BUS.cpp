#include "../include/BUS.h"



BUS::BUS() {
    // Constructor
    fill(begin(memory), end(memory), 0);
}


BUS::~BUS() {
    // Destructor
    
}

uint8_t BUS::busRead(uint16_t address) {
    if (address <= 0x1FFF) {
        return memory[address & 0x07FF];
    }
    else if (address <= 0x3FFF) {
        return memory[0x2000 + (address & 0x0007)];
    }
    else {
        return memory[address];
    }
}

uint8_t BUS::busWrite(uint16_t address, uint8_t value) {
    if (address <= 0x1FFF) {
        memory[address & 0x07FF] = value;
    }
    else if (address <= 0x3FFF) {
        memory[0x2000 + (address & 0x0007)] = value;
    }
    else {
        memory[address] = value;
    }
    return 0;
}
