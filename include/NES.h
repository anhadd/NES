#ifndef NES_H
#define NES_H

#include <SDL2/SDL.h>
#include <iostream>

#include "../include/GUI.h"
#include "../include/CPU.h"
#include "../include/ROM.h"
#include "../include/PPU.h"
#include "../include/BUS.h"

using namespace std;


class NES {
    public:
        CPU cpu;
        ROM rom;
        BUS bus;

        uint32_t total_cycles;      // Counts the total cycles since the start of the program.

        NES();
        ~NES();

        uint8_t initialize(char* romName);
        void executeCycle();
};



#endif
