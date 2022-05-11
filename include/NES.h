#ifndef NES_H
#define NES_H

#include <SDL2/SDL.h>
#include <iostream>

#include "GUI.h"
#include "CPU.h"
#include "ROM.h"
#include "PPU.h"
#include "BUS.h"


#define INPUT_A SDLK_z
#define INPUT_B SDLK_x

#define INPUT_START SDLK_RETURN
#define INPUT_SELECT SDLK_LSHIFT

#define INPUT_UP SDLK_UP
#define INPUT_DOWN SDLK_DOWN
#define INPUT_LEFT SDLK_LEFT
#define INPUT_RIGHT SDLK_RIGHT

#define INPUT_PALETTE SDLK_p
#define INPUT_SLOW SDLK_9
#define INPUT_FAST SDLK_0

using namespace std;


class NES {
    public:
        CPU cpu;
        ROM rom;
        BUS bus;
        PPU ppu;
        GUI gui;

        uint32_t total_cycles;      // Counts the total cycles since the start of the program.

        NES();
        ~NES();

        uint8_t initialize(char* romName);
        void executeFrame();
};



#endif
