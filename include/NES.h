#ifndef NES_H
#define NES_H

#include <SDL2/SDL.h>
#include <iostream>

#include "GUI.h"
#include "CPU.h"
#include "ROM.h"
#include "PPU.h"
#include "BUS.h"


#define INPUT_A SDL_SCANCODE_Z
#define INPUT_B SDL_SCANCODE_X

#define INPUT_START SDL_SCANCODE_RETURN
#define INPUT_SELECT SDL_SCANCODE_LSHIFT

#define INPUT_UP SDL_SCANCODE_UP
#define INPUT_DOWN SDL_SCANCODE_DOWN
#define INPUT_LEFT SDL_SCANCODE_LEFT
#define INPUT_RIGHT SDL_SCANCODE_RIGHT

#define INPUT_PALETTE SDLK_p
#define INPUT_SLOW SDLK_9
#define INPUT_FAST SDLK_8

#define INPUT_QUIT SDLK_ESCAPE

using namespace std;


class NES {
    public:
        CPU cpu;
        ROM rom;
        BUS bus;
        PPU ppu;
        GUI gui;

        uint32_t total_cycles;      // Counts the total cycles since the start of the program.

        const uint8_t* key_state;

        NES();
        ~NES();

        uint8_t initialize(char* romName);
        void executeFrame();
};



#endif
