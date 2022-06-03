#ifndef MAPPER_H
#define MAPPER_H

#include <SDL2/SDL.h>
#include <iostream>

#define PRG_BANK_SIZE 0x4000

using namespace std;

enum Mirroring {
    MIRROR_VERTICAL,
    MIRROR_HORIZONTAL,
    MIRROR_SINGLE_LOWER,
    MIRROR_SINGLE_UPPER
    // MIRROR_4_SCREEN
};

class Mapper {
    public:
        uint8_t id;
        uint8_t PRG_banks;
        uint8_t CHR_banks;

        enum Mirroring mirroring;
        bool prg_ram_enabled;

        Mapper(uint8_t nPRG = 0, uint8_t nCHR = 0);
        ~Mapper();

        virtual uint32_t cpuMap(uint16_t address, bool write, uint8_t value = 0x00) = 0;
        virtual uint32_t ppuMap(uint16_t address, bool write, uint8_t value = 0x00) = 0;
};


#endif
