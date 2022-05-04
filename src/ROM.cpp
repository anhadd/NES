#include "../include/ROM.h"



ROM::ROM() {
    // Constructor
    mirroring = 0;
}


ROM::~ROM() {
    // Destructor
    
}


void ROM::dumpContents(ifstream* romFile) {
    uint8_t x = 0;
    uint32_t counter = 0;

    fprintf(stderr, "ROM CONTENTS: \n");
    while (*romFile >> x) {
        fprintf(stderr, "%02x ", x);
        counter += 1;
        if (counter % 16 == 0) {
            fprintf(stderr, "\n");
        }
    }

    romFile->clear();
    romFile->seekg(0);
}


bool ROM::loadRom(char* romName, uint8_t (&memory)[MEMORY_ARRAY_SIZE], uint8_t (&ppu_patterntable)[0x2000]) {
    ifstream romFile(romName, ios::in | ios::binary);

    if (romFile.fail()) {
        return 1;
    }

    romFile.read(reinterpret_cast<char*>(h.full), ROM_HEADER_SIZE);

    // TODO: Check what to do with the trainer, right now it is just skipped.
    if (h.f6.trainer_present) {
        romFile.ignore(TRAINER_SIZE);
    }
    
    uint16_t buff_size = h.prg_rom_size * PRG_BLOCK_SIZE;
    uint8_t buff[buff_size];
    
    romFile.read(reinterpret_cast<char*>(buff), buff_size);
    memcpy(&memory[0x8000], buff, buff_size * sizeof(char));
    if (h.prg_rom_size == 1) {
        memcpy(&memory[0xC000], buff, buff_size * sizeof(char));
    }

    uint16_t buff_size2 = h.chr_rom_size * CHR_BLOCK_SIZE;
    uint8_t buff2[buff_size2];

    romFile.read(reinterpret_cast<char*>(buff2), buff_size2);
    memcpy(&ppu_patterntable[0], buff2, buff_size2 * sizeof(char));

    mirroring = h.f6.mirroring;

    romFile.close();
    return 0;
}
