#ifndef Mapper2_H
#define Mapper2_H

#include <SDL2/SDL.h>
#include <iostream>

#include "Mapper.h"

using namespace std;

// Mapper2 allows the PRG banks to be switched.
class Mapper2 : public Mapper {
    public:
        Mapper2(uint8_t nPRG, uint8_t nCHR);
        ~Mapper2();

        uint32_t cpuMap(uint16_t address, bool write, uint8_t value = 0x00);
        uint32_t ppuMap(uint16_t address, bool write, uint8_t value = 0x00);

    private:
        uint8_t prg_bank0;          // Stores the number of the PRG bank corresponding to 0x8000-0xBFFF.
        uint8_t prg_bank1;          // Stores the number of the PRG bank corresponding to 0xC000-0xFFFF.
        
};


#endif
