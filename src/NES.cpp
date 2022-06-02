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
    run_frame = false;
    FPS = 60;

    debug_log = false;

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
        if (cpu.cycles == 0 && debug_log && !bus.oam_writing) {
                fprintf(stderr, "%04x  %02x  %s  %02x %02x             A:%02x X:%02x Y:%02x P:%02x SP:%02x PPU: %03d,%03d\n", //  CYC:%u 
                        cpu.PC, 
                        cpu.cpuRead(cpu.PC), 
                        cpu.op_lookup[cpu.cpuRead(cpu.PC)].opname, 
                        cpu.cpuRead(cpu.PC + 1),
                        cpu.cpuRead(cpu.PC + 2),
                        cpu.accumulator, 
                        cpu.X, 
                        cpu.Y, 
                        cpu.status.full,
                        cpu.SP, 
                        ppu.scanlines, 
                        ppu.cycles
                        // cpu.total_cycles
                        );
        }

        ppu.executeCycle();
        ppu.executeCycle();
        ppu.executeCycle();
        
        if (!bus.oam_writing) {
            if (ppu.signal_nmi) {
                cpu.execute_nmi = true;
                ppu.signal_nmi = false;
            }
            cpu.executeCycle();
        }
        else {
            if (bus.cpu_synchronized) {
                if (total_cycles % 2 == 0) {
                    bus.oam_data = bus.busReadCPU((bus.oam_page << 8) | bus.oam_index);
                }
                else {
                    ppu.writeRegister(OAM_DATA, bus.oam_data);
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

        total_cycles += 1;
    }
    ppu.finished = false;
}
