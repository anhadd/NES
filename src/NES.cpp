#include "../include/NES.h"



NES::NES() {
    // Constructor
    cpu.passBUS(&bus);
}


NES::~NES() {
    // Destructor
}

uint8_t NES::initialize(char* romName) {
    if (rom.loadRom(romName, bus.memory) != 0) {
        return 1;
    }
    printf("Rom Loaded!\n");

    cpu.reset();
    return 0;
}

void NES::executeCycle() {
    if (total_cycles % 3 == 0) {
        cpu.executeCycle();
    }
    // ppu.executeCycle();
    total_cycles += 1;
}
