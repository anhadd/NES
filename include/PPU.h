#ifndef PPU_H
#define PPU_H

#include <SDL2/SDL.h>
#include <iostream>

#include "BUS.h"

using namespace std;


class PPU {
    public:
        BUS* bus;
        

        PPU();
        ~PPU();

        void passBUS(BUS* nesBUS);

        uint8_t ppuRead(uint16_t address);
        uint8_t ppuWrite(uint16_t address, uint8_t value);

        bool executeCycle();
};



#endif
