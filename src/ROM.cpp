#include "../include/ROM.h"



ROM::ROM() {
    // Constructor
    
}


ROM::~ROM() {
    // Destructor
    
}


bool ROM::loadRom(char* romName, uint8_t (&memory)[65536]) {
    ifstream romFile(romName, ios::in | ios::binary);

    if (romFile.fail()) {
        return 1;
    }
    romFile.read(reinterpret_cast<char*>(h.full), ROM_HEADER_SIZE);
    
    uint16_t buff_size = h.prg_rom_size * PRG_BLOCK_SIZE;
    uint8_t buff[buff_size];
    romFile.read(reinterpret_cast<char*>(buff), buff_size);
    memcpy(&memory[0x8000], buff, buff_size * sizeof(char));

    if (h.prg_rom_size == 1) {
        memcpy(&memory[0xC000], buff, buff_size * sizeof(char));
    }

    romFile.close();
    return 0;
}
