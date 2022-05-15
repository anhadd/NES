#ifndef BUS_H
#define BUS_H

#include <SDL2/SDL.h>
#include <iostream>

#include "PPU.h"

using namespace std;

class BUS {
    public:
        // TODO: lower the size to remove the mirroring parts of memory for space.
        uint8_t memory[0x10000];        // CPU memory

        // Used for handling input.
        uint8_t controller[2];
        uint8_t controller_shift[2];

        PPU* ppu;
        
        BUS();
        ~BUS();

        void passPPU(PPU* nesPPU);

        uint8_t busReadCPU(uint16_t address);
        uint8_t busWriteCPU(uint16_t address, uint8_t value);

        uint8_t busReadPPU(uint16_t address);
        uint8_t busWritePPU(uint16_t address, uint8_t value);
};


#endif
