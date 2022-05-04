#include "../include/NES.h"



NES::NES() {
    // Constructor
    ppu.passGUI(&gui);

    bus.passPPU(&ppu);
    cpu.passBUS(&bus);
}


NES::~NES() {
    // Destructor
}

uint8_t NES::initialize(char* romName) {
    if (rom.loadRom(romName, bus.memory, ppu.ppu_patterntable) != 0) {
        return 1;
    }

    cpu.reset();
    ppu.reset();

    ppu.vertical_mirorring = rom.h.f6.mirroring;
    return 0;
}

void NES::executeFrame() {
    while (!ppu.finished) {
        ppu.executeCycle();
        if (total_cycles % 3 == 0) {
            cpu.executeCycle();
        }

        if (ppu.signal_nmi) {
            ppu.signal_nmi = false;
            cpu.NMI();
        }

        total_cycles += 1;
    }
    ppu.finished = false;
}
