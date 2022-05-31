#include "../include/BUS.h"



BUS::BUS() {
    // Constructor
    fill(begin(memory), end(memory), 0);

    fill(begin(controller), end(controller), 0);
    fill(begin(controller_shift), end(controller_shift), 0);
    
    oam_page = 0x00;
    oam_index = 0x00;
    oam_writing = false;
    cpu_synchronized = false;
}

BUS::~BUS() {
    // Destructor
    
}

void BUS::reset() {
    // fill(begin(memory), &memory[0x7FF], 0);
    
    fill(begin(controller), end(controller), 0);
    fill(begin(controller_shift), end(controller_shift), 0);

    oam_page = 0x00;
    oam_index = 0x00;
    oam_writing = false;
    cpu_synchronized = false;
}

void BUS::passPPU(PPU* nesPPU) {
    ppu = nesPPU;
}

void BUS::passROM(ROM* nesROM) {
    rom = nesROM;
}


// CPU reading from the BUS.
uint8_t BUS::busReadCPU(uint16_t address) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        return memory[address & 0x07FF];
    }
    else if (address >= 0x2000 && address <= 0x3FFF) {
        return ppu->readRegister(address);
    }
    else if (address == 0x4016 || address == 0x4017) {
        uint8_t temp = controller_shift[address % 0x4016];
        controller_shift[address % 0x4016] <<= 1;
        return (temp & 0x80) != 0;
    }
    else if (address >= 0x6000 && address <= 0x7FFF) {
        // TODO: CHECK WHY SOME MAPPER 2 ROMS USE THIS (THOUGHT THEY SHOULDNT ?)
        // fprintf(stderr, "!!!!! PRG RAM USED !!!!!!\n");
        return rom->PRG_ram[rom->mapper->cpuMap(address, false)];
    }
    else if (address >= 0x8000 && address <= 0xFFFF) {
        return rom->PRG_memory[rom->mapper->cpuMap(address, false)];
    }
    return 0x00;
}

// CPU writing to the BUS.
uint8_t BUS::busWriteCPU(uint16_t address, uint8_t value) {
    if (address >= 0x0000 && address <= 0x1FFF) {
        memory[address & 0x07FF] = value;
    }
    else if (address >= 0x2000 && address <= 0x3FFF) {
        ppu->writeRegister(address, value);
    }
    else if (address == 0x4014) {
        oam_page = value;
        oam_index = 0x00;
        oam_writing = true;
    }
    else if (address == 0x4016 || address == 0x4017) {
        controller_shift[address % 0x4016] = controller[address % 0x4016];
    }
    else if (address >= 0x6000 && address <= 0x7FFF) {
        // TODO: CHECK WHY SOME MAPPER 2 ROMS USE THIS (THOUGHT THEY SHOULDNT ?)
        // fprintf(stderr, "!!!!! PRG RAM USED !!!!!!\n");
        rom->PRG_ram[rom->mapper->cpuMap(address, true, value)] = value;
    }
    else if (address >= 0x8000 && address <= 0xFFFF) {
        // Writing not allowed for PRG memory.
        rom->mapper->cpuMap(address, true, value);
    }
    return 0;
}
