#ifndef BUS_H
#define BUS_H

#include <SDL2/SDL.h>
#include <iostream>

#include "PPU.h"

using namespace std;

class BUS {
    public:
        // TODO: add "-" to remove the mirroring parts of memory for space.
        // TODO: FIGURE OUT HOW I WANNA IMPLEMENT THE MEMORY STUFF (SEPARATE OR RANGES ?) 
        uint8_t memory[0x10000];        // CPU memory

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
