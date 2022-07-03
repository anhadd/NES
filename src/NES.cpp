#include "../include/NES.h"



NES::NES() {
    // Constructor
    // Pass the necessary components to each class.
    ppu.passGUI(&gui);
    ppu.passROM(&rom);

    bus.passPPU(&ppu);
    bus.passROM(&rom);

    cpu.passBUS(&bus);

    // Initialize other variables.
    key_state = SDL_GetKeyboardState(NULL);

    quit = false;
    paused = false;
    run_frame = false;
    FPS = 60;

    debug_log = false;

    // Tell the PPU whether to render debug screens.
    ppu.show_debug = SHOW_DEBUG;
    // Create the debug windows if debugging is turned on.
    if (SHOW_DEBUG) {
        gui.createDebugWindows();
    }
}


NES::~NES() {
    // Destructor
}

uint8_t NES::initialize(string romName) {
    // Load the ROM. Returns 1 on failure.
    if (rom.loadRom(romName) != 0) {
        return 1;
    }
    reset();
    return 0;
}

void NES::reset() {
    total_cycles = 0;
    cpu.reset();
    bus.reset();
    rom.reset();
    ppu.reset();
}

void NES::transferOAM() {
    // If the CPU is synchronized (want to start at an even cycle).
    if (bus.cpu_synchronized) {
        // Every even cycle, load new data into oam_data.
        if (total_cycles % 2 == 0) {
            bus.oam_data = bus.busReadCPU((bus.oam_page << 8) | bus.oam_index);
        }
        // Every odd cycle, write that data to the PPU OAM.
        else {
            ppu.writeRegister(OAM_DATA, bus.oam_data);
            // If an entire page has been written, OAM writing is done so reset everything.related to that.
            if (bus.oam_index == 0xFF) {
                bus.oam_page = 0x00;
                bus.oam_data = 0x00;
                bus.oam_index = 0x00;
                bus.oam_writing = false;
                bus.cpu_synchronized = false;
            }
            // Otherwise increase the current index and continue.
            else {
                bus.oam_index += 1;
            }
        }
    }
    // Try to synchronize with the CPU.
    else {
        if (total_cycles % 2 == 1) {
            bus.cpu_synchronized = true;
        }
    }
}

void NES::logDebugInfo() {
    // Prints debug log information if logging is enabled.
    if (cpu.cycles == 0 && debug_log && !bus.oam_writing) {
        fprintf(stderr, "%04x  %02x  %s  %02x %02x             A:%02x X:%02x Y:%02x P:%02x SP:%02x PPU: %03d,%03d CYC:%u\n",
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
            ppu.cycles,
            cpu.total_cycles
        );
    }
}

void NES::executeFrame() {
    // Execute cycles until the PPU has rendered an entire frame.
    while (!ppu.frame_finished) {
        // Logs info.
        logDebugInfo();

        // Execute PPU cycles. There are 3 PPU cycles for each CPU cycle.
        ppu.executeCycle();
        ppu.executeCycle();
        ppu.executeCycle();
        // If there is no writing happening to OAM.
        if (!bus.oam_writing) {
            // If PPU is asking for an NMI send that to CPU.
            if (ppu.signal_nmi) {
                cpu.execute_nmi = true;
                ppu.signal_nmi = false;
            }
            // Run a single CPU cycle.
            cpu.executeCycle();
        }
        // If OAM is being written to.
        else {
            // Read or write 1 byte of data for OAM.
            transferOAM();
        }

        total_cycles += 1;
    }
    ppu.frame_finished = false;
}
