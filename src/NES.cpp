#include "../include/NES.h"



NES::NES() {
    // Constructor
    ppu.passGUI(&gui);
    ppu.passROM(&rom);

    bus.passPPU(&ppu);
    bus.passROM(&rom);
    cpu.passBUS(&bus);

    key_state = SDL_GetKeyboardState(NULL);

    quit = false;
    paused = false;
    FPS = 60;

    ppu.show_debug = SHOW_DEBUG;

    if (SHOW_DEBUG) {
        gui.createDebugWindows();
    }
}


NES::~NES() {
    // Destructor
}

uint8_t NES::initialize(char* romName) {
    if (rom.loadRom(romName) != 0) {
        return 1;
    }

    reset();

    ppu.vertical_mirorring = rom.h.f6.mirroring;
    return 0;
}

void NES::reset() {
    total_cycles = 0;
    cpu.reset();
    bus.reset();
    rom.reset();
    ppu.reset();
}

void NES::executeFrame() {
    while (!ppu.finished) {
        // TODO: COULD JUST RUN 3 PPU CYCLES THEN 1 CPU, CHECK WHICH IS BETTER.
        ppu.executeCycle();
        if (total_cycles % 3 == 0) {
            if (!bus.oam_writing) {
                // if (cpu.cycles == 0) {
                //     fprintf(stderr, "%04x  %02x  %s             A:%02x X:%02x Y:%02x P:%02x SP:%02x PPU: %03d,%03d CYC:%u\n", 
                //             cpu.PC, 
                //             cpu.cpuRead(cpu.PC), 
                //             cpu.op_lookup[cpu.cpuRead(cpu.PC)].opname, 
                //             cpu.accumulator, 
                //             cpu.X, 
                //             cpu.Y, 
                //             cpu.status.full, 
                //             cpu.SP, 
                //             ppu.scanlines, 
                //             ppu.cycles - 1, 
                //             cpu.total_cycles);
                // }
                // TODO: BM FREEZES ON TITLE SCREEN IF CALLING NMI IS PUT HERE, CHECK WHY AND WHETHER TO FIX THAT
                // if (cpu.cycles == 0 && ppu.signal_nmi) { // TODO: CHECK WHERE NMI SHOULD BE IN HERE, OR WHETHER IT EVEN MATTERS.
                //     ppu.signal_nmi = false;
                //     cpu.NMI();
                // }
                // else {
                    cpu.executeCycle();
                // }
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

        // TODO: BM STILL FREEZES ON TITLE SCREEN SOMETIMES WITH NMI CALLED HERE, SO CHECK HOW TO FIX THAT
        if (ppu.signal_nmi) { // TODO: CHECK WHERE NMI SHOULD BE CALLED, OR WHETHER IT EVEN MATTERS.
            ppu.signal_nmi = false;
            cpu.NMI();
        }

        total_cycles += 1;
    }
    ppu.finished = false;
}
