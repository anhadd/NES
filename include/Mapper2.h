#ifndef Mapper2_H
#define Mapper2_H

#include <SDL2/SDL.h>
#include <iostream>

#include "Mapper.h"

using namespace std;

class Mapper2 : public Mapper {
    public:
        Mapper2(uint8_t nPRG, uint8_t nCHR);
        ~Mapper2();

        uint32_t cpuMap(uint16_t address, bool write, uint8_t value = 0x00);
        uint32_t ppuMap(uint16_t address, bool write, uint8_t value = 0x00);

    private:
        uint8_t prg_bank0;
        uint8_t prg_bank1;
};


#endif
