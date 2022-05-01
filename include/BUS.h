#ifndef BUS_H
#define BUS_H

#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

#define CONTROL     0x2000
#define MASK        0x2001
#define STATUS      0x2002
#define OAM_ADDR    0x2003
#define OAM_DATA    0x2004
#define SCROLL      0x2005
#define PPU_ADDR    0x2006
#define PPU_DATA    0x2007

class BUS {
    public:
        // TODO: add "-" to remove the mirroring parts of memory for space.
        // TODO: FIGURE OUT HOW I WANNA IMPLEMENT THE MEMORY STUFF (SEPARATE OR RANGES ?) 
        uint8_t memory[0x10000];        // CPU memory
        
        uint8_t ppu_patterntable[0x2000];       // PPU memory
        uint8_t ppu_nametable[0x1000];          // PPU memory
        uint8_t ppu_palette[0x0020];            // PPU memory
        
        BUS();
        ~BUS();

        uint8_t busReadCPU(uint16_t address);
        uint8_t busWriteCPU(uint16_t address, uint8_t value);

        uint8_t busReadPPU(uint16_t address);
        uint8_t busWritePPU(uint16_t address, uint8_t value);
};


#endif
