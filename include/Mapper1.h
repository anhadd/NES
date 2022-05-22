#ifndef MAPPER1_H
#define MAPPER1_H

#include <SDL2/SDL.h>
#include <iostream>

#include "Mapper.h"

using namespace std;

class Mapper1 : public Mapper {
    public:
        Mapper1();
        ~Mapper1();

        uint32_t cpuMap(uint16_t address, bool write, uint8_t value = 0x00);
        uint32_t ppuMap(uint16_t address);
};


#endif
