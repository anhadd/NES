#ifndef MAPPER0_H
#define MAPPER0_H

#include <SDL2/SDL.h>
#include <iostream>

#include "Mapper.h"

using namespace std;

class Mapper0 : public Mapper {
    public:
        Mapper0();
        ~Mapper0();

        uint32_t cpuMap(uint16_t address, bool write, uint8_t value = 0x00);
        uint32_t ppuMap(uint16_t address);
};


#endif
