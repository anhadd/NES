#include "../include/NES.h"



NES::NES() {
    // Constructor
    ppu.passGUI(&gui);

    bus.passPPU(&ppu);
    cpu.passBUS(&bus);

    key_state = SDL_GetKeyboardState(NULL);

    ppu.show_debug = SHOW_DEBUG;

    if (SHOW_DEBUG) {
        gui.createDebugWindows();
    }
}


NES::~NES() {
    // Destructor
}

uint8_t NES::initialize(char* romName) {
    vector<uint8_t>* ptr = &bus.PRG_memory;
    tie(bus.PRG_memory, ppu.CHR_memory) = rom.loadRom(romName);
    if (bus.PRG_memory == vector<uint8_t>() || ppu.CHR_memory == vector<uint8_t>()) {
        return 1;
    }
    
    cpu.reset();
    ppu.reset();

    ppu.vertical_mirorring = rom.h.f6.mirroring;
    return 0;
}

void NES::reset() {
    cpu.reset();
    ppu.reset();
}

void NES::executeFrame() {
    while (!ppu.finished) {
        ppu.executeCycle();
        if (total_cycles % 3 == 0) {
            if (!bus.oam_writing) {
                cpu.executeCycle();
            }
            else {
                if (bus.cpu_synchronized) {
                    if (total_cycles % 2 == 0) {
                        bus.oam_data = bus.busReadCPU((bus.oam_page << 8) | bus.oam_index);
                    }
                    else {
                        ppu.OAM[bus.oam_index] = bus.oam_data;
                        if (bus.oam_index == 0xFF) {
                            bus.oam_page = 0x00;
                            bus.oam_data = 0x00;
                            bus.oam_index = 0x00;
                            bus.oam_writing = false;
                            bus.cpu_synchronized = false;
                        }
                        else {
                            bus.oam_index += 1;
                        }
                    }
                }
                else {
                    if (total_cycles % 2 == 1) {
                        bus.cpu_synchronized = true;
                    }
                }
            }
            
        }

        if (ppu.signal_nmi) {
            ppu.signal_nmi = false;
            cpu.NMI();
        }

        total_cycles += 1;
    }
    ppu.finished = false;
}
