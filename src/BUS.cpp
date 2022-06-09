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
    // Read from CPU RAM.
    if (address >= 0x0000 && address <= 0x1FFF) {
        return memory[address & 0x07FF];
    }
    // Read from PPU registers.
    else if (address >= 0x2000 && address <= 0x3FFF) {
        return ppu->readRegister(address);
    }
    // Read from audio registers.
    else if (address >= 0x4000 && address <= 0x4015) {
        // TODO: Audio registers are not implemented.
    }
    // Read from controller registers.
    else if (address == 0x4016 || address == 0x4017) {
        uint8_t temp = controller_shift[address % 0x4016];
        controller_shift[address % 0x4016] <<= 1;
        return (temp & 0x80) != 0;
    }
    // Read from additional APU and I/O registers.
    else if(address >= 0x4018 && address <= 0x401F) {
        // TODO: APU and I/O functionality that is normally disabled.
    }
    // Read from expansion ROM.
    else if (address >= 0x4020 && address <= 0x5FFF) {
        return rom->Expansion_ROM[address % 0x4020];
    }
    // Read from PRG RAM.
    else if (address >= 0x6000 && address <= 0x7FFF) {
        // TODO: CHECK WHY SOME MAPPER 2 ROMS USE THIS (THOUGHT THEY SHOULDNT ?)
        return rom->PRG_ram[rom->mapper->cpuMap(address, false)];
    }
    // Read from PRG memory / PRG ROM.
    else if (address >= 0x8000 && address <= 0xFFFF) {
        return rom->PRG_memory[rom->mapper->cpuMap(address, false)];
    }
    return 0x00;
}

// CPU writing to the BUS.
uint8_t BUS::busWriteCPU(uint16_t address, uint8_t value) {
    // Write to CPU RAM.
    if (address >= 0x0000 && address <= 0x1FFF) {
        memory[address & 0x07FF] = value;
    }
    // Write to PPU registers.
    else if (address >= 0x2000 && address <= 0x3FFF) {
        ppu->writeRegister(address, value);
    }
    // Writes to 0x4014 initiates writing data into OAM.
    else if (address == 0x4014) {
        oam_page = value;
        oam_index = 0x00;
        oam_writing = true;
    }
    // Write to audio registers
    else if (address >= 0x4000 && address <= 0x4015 && address != 0x4014) {
        // TODO: Audio registers are not implemented.
    }
    // Writes to controller registers save the current controller state to be read later.
    else if (address == 0x4016 || address == 0x4017) {
        controller_shift[address % 0x4016] = controller[address % 0x4016];
    }
    // Write to additional APU and I/O registers.
    else if(address >= 0x4018 && address <= 0x401F) {
        // TODO: APU and I/O functionality that is normally disabled.
    }
    // Write to expansion ROM.
    else if (address >= 0x4020 && address <= 0x5FFF) {
        rom->Expansion_ROM[address % 0x4020] = value;
    }
    // Write to PRG RAM.
    else if (address >= 0x6000 && address <= 0x7FFF) {
        // TODO: CHECK WHY SOME MAPPER 2 ROMS USE THIS (THOUGHT THEY SHOULDNT ?)
        rom->PRG_ram[rom->mapper->cpuMap(address, true, value)] = value;
    }
    // Write to PRG memory / PRG ROM.
    // Writing to ROM is not allowed, however initiating a write might alter the internals of the mapper.
    else if (address >= 0x8000 && address <= 0xFFFF) {
        rom->mapper->cpuMap(address, true, value);
    }
    return 0;
}
