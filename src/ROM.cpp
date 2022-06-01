#include "../include/ROM.h"



ROM::ROM() {
    // Constructor
    fill(begin(h.full), end(h.full), 0);
    mapper = nullptr;
    mapper_id = 0;

    // fill(begin(PRG_memory), end(PRG_memory), 0);
    // fill(begin(PRG_ram), end(PRG_ram), 0);
    // fill(begin(CHR_memory), end(CHR_memory), 0);
    // fill(begin(Expansion_ROM), end(Expansion_ROM), 0);
}


ROM::~ROM() {
    // Destructor
    
}


void ROM::reset() {
    fill(begin(PRG_ram), end(PRG_ram), 0);
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

    // Trainer data is skipped.
    if (h.f6.trainer_present) {
        printf("WARNING: Mapper in ROM file!\n");
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
    Expansion_ROM.resize(0x1FE0);
    fill(begin(PRG_ram), end(PRG_ram), 0);
    fill(begin(Expansion_ROM), end(Expansion_ROM), 0);

    mapper_id = (h.f7.mapper_upper << 4) | (h.f6.mapper_lower);
    switch(mapper_id) {
        case 0:
            printf("Mapper 0\n");
            mapper = new Mapper0(h.prg_rom_size, h.chr_rom_size);
            break;
        case 1:
            printf("Mapper 1\n");
            mapper = new Mapper1(h.prg_rom_size, h.chr_rom_size);
            break;
        case 2:
            printf("Mapper 2\n");
            mapper = new Mapper2(h.prg_rom_size, h.chr_rom_size);
            break;
        case 3:
            printf("Mapper 3\n");
            mapper = new Mapper3(h.prg_rom_size, h.chr_rom_size);
            break;
        default:
            printf("Error: Unsupported Mapper %u\n", mapper_id);
            exit(0);
    }

    romFile.close();
    return 0;
}
