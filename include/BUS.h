#ifndef BUS_H
#define BUS_H

#include <SDL2/SDL.h>
#include <iostream>

using namespace std;


class BUS {
    public:
        // TODO: add "-" to remove the mirroring parts of memory for space.
        // TODO: FIGURE OUT HOW I WANNA IMPLEMENT THE MEMORY STUFF (SEPARATE OR RANGES ?) 
        uint8_t memory[0x10000];        // CPU memory
        
        // uint8_t v_ram[0x10000];         // PPU memory
        // uint8_t spr_ram[0x100];           // Stores sprite attributes.
        
        BUS();
        ~BUS();

        uint8_t busRead(uint16_t address);
        uint8_t busWrite(uint16_t address, uint8_t value);
};


#endif
