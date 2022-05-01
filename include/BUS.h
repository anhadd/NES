#ifndef BUS_H
#define BUS_H

#include <SDL2/SDL.h>
#include <iostream>

#include "CPU.h"
#include "PPU.h"

using namespace std;

class BUS {
    public:
        CPU* cpu;
        PPU* ppu;
        
        BUS();
        ~BUS();

        void passPPU(PPU* nesPPU);
        void passCPU(CPU* nesCPU);

        uint8_t busReadCPU(uint16_t address);
        uint8_t busWriteCPU(uint16_t address, uint8_t value);

        uint8_t busReadPPU(uint16_t address);
        uint8_t busWritePPU(uint16_t address, uint8_t value);
};


#endif
