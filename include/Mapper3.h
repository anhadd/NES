#ifndef Mapper3_H
#define Mapper3_H

#include <SDL2/SDL.h>
#include <iostream>

#include "Mapper.h"

using namespace std;

// Mapper3 allows the CHR banks to be switched.
class Mapper3 : public Mapper {
    public:
        Mapper3(uint8_t nPRG, uint8_t nCHR);
        ~Mapper3();

        uint32_t cpuMap(uint16_t address, bool write, uint8_t value = 0x00);
        uint32_t ppuMap(uint16_t address, bool write, uint8_t value = 0x00);

    private:
        uint8_t chr_bank0;          // Stores the number of the CHR bank corresponding to 0x0000-0x1FFF.
};


#endif
