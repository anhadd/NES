#ifndef MAPPER_H
#define MAPPER_H

#include <SDL2/SDL.h>
#include <iostream>

#define PRG_BANK_SIZE 0x4000

using namespace std;

class Mapper {
    public:
        uint8_t id;
        uint8_t PRG_banks;
        uint8_t CHR_banks;

        uint8_t selected_bank1;
        uint8_t selected_bank2;

        Mapper();
        ~Mapper();

        virtual uint32_t cpuMap(uint16_t address, bool write, uint8_t value = 0x00) = 0;
        virtual uint32_t ppuMap(uint16_t address) = 0;
};


#endif
