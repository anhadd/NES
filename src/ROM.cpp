#include "../include/ROM.h"



ROM::ROM() {
    // Constructor
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


pair<vector<uint8_t>, vector<uint8_t>> ROM::loadRom(char* romName) {
    vector<uint8_t> PRG_memory;
    vector<uint8_t> CHR_memory;
    ifstream romFile(romName, ios::in | ios::binary);

    if (romFile.fail()) {
        return make_pair(vector<uint8_t>(), vector<uint8_t>());
    }

    romFile.read(reinterpret_cast<char*>(h.full), ROM_HEADER_SIZE);

    // TODO: Check what to do with the trainer, right now it is just skipped.
    // TODO: ADD MAPPER SUPPORT !!!!
    if (h.f6.trainer_present) {
        romFile.ignore(TRAINER_SIZE);
    }
    
    // Buffer for the PRG data.
    uint16_t buff_size = h.prg_rom_size * PRG_BLOCK_SIZE;
    uint8_t buff[buff_size];

    // Buffer for the CHR data.
    uint16_t buff2_size = h.chr_rom_size * CHR_BLOCK_SIZE;
    uint8_t buff2[buff2_size];
    
    // Load the PRG memory.
    romFile.read(reinterpret_cast<char*>(buff), buff_size);
    PRG_memory.resize(buff_size);
    memcpy(&PRG_memory[0x0000], buff, buff_size * sizeof(char));
    // TODO: if necessary, just copy the 1 bank again so mapping can be the same.
    // if (h.prg_rom_size == 1) {
    //     memcpy(&PRG_memory->at(0x0000), buff, buff_size * sizeof(char));
    // }

    // Load the CHR memory.
    romFile.read(reinterpret_cast<char*>(buff2), buff2_size);
    CHR_memory.resize(buff2_size);
    memcpy(&CHR_memory[0x0000], buff2, buff2_size * sizeof(char));

    mapper.CHR_banks = h.chr_rom_size;
    mapper.PRG_banks = h.prg_rom_size;

    romFile.close();
    return make_pair(PRG_memory, CHR_memory);
}
