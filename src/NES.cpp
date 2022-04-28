#include "../include/NES.h"



NES::NES() {
    // Constructor
    cpu.passBUS(&bus);
    ppu.passBUS(&bus);
    ppu.passGUI(&gui);
}


NES::~NES() {
    // Destructor
}

uint8_t NES::initialize(char* romName) {
    if (rom.loadRom(romName, bus.memory) != 0) {
        return 1;
    }

    cpu.reset();
    ppu.reset();
    return 0;
}

void NES::executeFrame() {
    while (!ppu.finished) {
        ppu.executeCycle();
        if (total_cycles % 3 == 0) {
            cpu.executeCycle();
        }
        total_cycles += 1;
    }
    ppu.finished = false;
}
