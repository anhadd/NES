#ifndef BUS_H
#define BUS_H

#include <SDL2/SDL.h>
#include <iostream>

using namespace std;


class BUS {
    public:
        uint8_t memory[0x10000];    // TODO: add "-" to remove the mirroring parts of memory.
        
        BUS();
        ~BUS();

        uint8_t busRead(uint16_t address);
        uint8_t busWrite(uint16_t address, uint8_t value);
};



#endif
