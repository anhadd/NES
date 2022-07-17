#include "../include/ROM.h"



ROM::ROM() {
    // Constructor
    fill(begin(h.full), end(h.full), 0);
    mapper = nullptr;
    mapper_id = 0;

    CHR_is_ram = false;
}


ROM::~ROM() {
    // Destructor
}


void ROM::reset() {
    // Resetting ROM does nothing.
}


vector<string> splitPath(const string &path_str) {
    std::stringstream ss(path_str);
    std::string element;
    std::vector<string> split_str;
    // Split the entire given ROM path by '/'.
    while (std::getline(ss, element, '/')) {
        split_str.push_back(element);
    }
    return split_str;
}

void ROM::loadSaveFile(string romName) {
    vector<string> path_elems = splitPath(romName);
    save_path = "saves/" + path_elems.back();
    if (access(save_path.c_str(), F_OK) != -1) {
        printf("Loading saved game...\n");
        uint32_t buff3_size = 0x2000;
        uint8_t buff3[buff3_size];

        ifstream save_file(save_path, ios::in | ios::binary);
        save_file.read(reinterpret_cast<char*>(buff3), buff3_size);
        memcpy(&PRG_ram[0x0000], buff3, buff3_size * sizeof(char));
        save_file.close();
    }
    else {
        printf("No save file found.\n");
    }
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
    // Go back to the start of the romFile.
    romFile->clear();
    romFile->seekg(0);
}


bool ROM::loadRom(string romName) {
    ifstream romFile(romName, ios::in | ios::binary);

    if (romFile.fail()) {
        return 1;
    }

    // Dump the ROM contents. Nice to have for debugging.
    // dumpContents(&romFile);

    // Read the header data.
    romFile.read(reinterpret_cast<char*>(h.full), ROM_HEADER_SIZE);

    // Trainer data is skipped.
    if (h.f6.trainer_present) {
        printf("Trainer found in ROM file, skipped.\n");
        romFile.ignore(TRAINER_SIZE);
    }
    
    // Buffer for the PRG data.
    uint32_t buff_size = h.prg_rom_size * PRG_BLOCK_SIZE;
    uint8_t buff[buff_size];

    // Buffer for the CHR data.
    uint32_t buff2_size = h.chr_rom_size * CHR_BLOCK_SIZE;
    uint8_t buff2[buff2_size];

    // If the amount of CHR banks is 0.
    if (h.chr_rom_size == 0) {
        // The CHR memory is RAM and the size is as if there is 1 CHR bank.
        CHR_is_ram = true;
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

    // Create PRG RAM and Expansion ROM. This is not necessarily used by every mapper.
    PRG_ram.resize(PRG_RAM_SIZE);
    Expansion_ROM.resize(EXPANSION_ROM_SIZE);
    fill(begin(PRG_ram), end(PRG_ram), 0);
    fill(begin(Expansion_ROM), end(Expansion_ROM), 0);

    // Create the Mapper depending on the id in the ROM header.
    mapper_id = (h.f7.mapper_upper << 4) | (h.f6.mapper_lower);
    switch(mapper_id) {
        case 0:
            printf("Mapper 0 - ");
            mapper = new Mapper0(h.prg_rom_size, h.chr_rom_size);
            break;
        case 1:
            printf("Mapper 1 - ");
            mapper = new Mapper1(h.prg_rom_size, h.chr_rom_size);
            break;
        case 2:
            printf("Mapper 2 - ");
            mapper = new Mapper2(h.prg_rom_size, h.chr_rom_size);
            break;
        case 3:
            printf("Mapper 3 - ");
            mapper = new Mapper3(h.prg_rom_size, h.chr_rom_size);
            break;
        default:
            printf("Error: Unsupported Mapper %u\n", mapper_id);
            exit(0);
    }
    printf("Prg banks: %u    Chr banks: %u\n", mapper->PRG_banks, mapper->CHR_banks);

    // If the mapper uses PRG RAM, it is allowed to use saves and loads, so load the game if possible.
    if (mapper->prg_ram_enabled) {
        loadSaveFile(romName);
    }

    // Set mirroring mode.
    if (h.f6.mirroring) {
        mapper->mirroring = MIRROR_VERTICAL;
    }
    else {
        mapper->mirroring = MIRROR_HORIZONTAL;
    }

    romFile.close();
    return 0;
}
