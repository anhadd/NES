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


bool ROM::loadRom(char* romName) {
    ifstream romFile(romName, ios::in | ios::binary);

    if (romFile.fail()) {
        return 1;
    }

    romFile.read(reinterpret_cast<char*>(h.full), ROM_HEADER_SIZE);

    // TODO: Check what to do with the trainer, right now it is just skipped.
    if (h.f6.trainer_present) {
        // TODO: Check if this doesnt jump too far (because of strating at 0x10)
        romFile.ignore(TRAINER_SIZE);
    }
    
    // Buffer for the PRG data.
    uint32_t buff_size = h.prg_rom_size * PRG_BLOCK_SIZE;
    uint8_t buff[buff_size];

    // Buffer for the CHR data.
    uint32_t buff2_size = h.chr_rom_size * CHR_BLOCK_SIZE;
    uint8_t buff2[buff2_size];

    if (h.chr_rom_size == 0) {
        buff2_size = CHR_BLOCK_SIZE;
    }

    // Load the PRG memory.
    romFile.read(reinterpret_cast<char*>(buff), buff_size);
    PRG_memory.resize(buff_size);
    memcpy(&PRG_memory[0x0000], buff, buff_size * sizeof(char));

    // Load the CHR memory.
    romFile.read(reinterpret_cast<char*>(buff2), buff2_size);
    CHR_memory.resize(buff2_size);
    memcpy(&CHR_memory[0x0000], buff2, buff2_size * sizeof(char));

    // TODO: Check if PRG RAM is really always necessary.
    PRG_ram.resize(0x2000);

    mapper_id = (h.f7.mapper_upper << 4) | (h.f6.mapper_lower);
    switch(mapper_id) {
        case 0:
            printf("Mapper 0\n");
            mapper = new Mapper0();
            break;
        case 1:
            printf("Mapper 1\n");
            mapper = new Mapper1();
            break;
        case 2:
            printf("Mapper 2\n");
            mapper = new Mapper2();
            mapper->selected_bank2 = h.prg_rom_size - 1;
            break;
        case 3:
            printf("Mapper 3\n");
            mapper = new Mapper3();
            
            break;
        default:
            printf("Unsupported Mapper!\n");
            exit(0);
    }

    mapper->CHR_banks = h.chr_rom_size;
    mapper->PRG_banks = h.prg_rom_size;

    romFile.close();
    return 0;
}
