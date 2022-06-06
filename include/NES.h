#ifndef NES_H
#define NES_H

#include <SDL2/SDL.h>
#include <iostream>

#include <chrono>
#include <thread>

#include "GUI.h"
#include "CPU.h"
#include "ROM.h"
#include "PPU.h"
#include "BUS.h"
#include "APU.h"


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
#define INPUT_RESET SDLK_r

#define INPUT_PAUSE SDLK_SPACE

#define INPUT_LOG SDLK_l
#define INPUT_FRAME SDLK_f
#define SHOW_DEBUG true

using namespace std;
using namespace std::this_thread;
using namespace std::chrono;


class NES {
    public:
        CPU cpu;
        BUS bus;
        PPU ppu;
        GUI gui;
        ROM rom;
        APU apu;

        bool quit;
        int FPS;

        bool paused;
        bool run_frame;
        bool debug_log;

        const uint8_t* key_state;

        // std::time_t next_cycle_time;
        uint32_t cycle_time_ns;

        NES();
        ~NES();

        uint8_t initialize(string romName);
        void executeFrame();
        void reset();

    private:
        uint32_t total_cycles;      // Counts the total cycles since the start of the program.
};



#endif
