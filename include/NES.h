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
#define INPUT_FAST SDLK_0

#define INPUT_QUIT SDLK_ESCAPE
#define INPUT_RESET SDLK_r

#define INPUT_PAUSE SDLK_SPACE

#define INPUT_LOG SDLK_l
#define INPUT_FRAME SDLK_f
#define SHOW_DEBUG false

using namespace std;


class NES {
    public:
        CPU cpu;
        BUS bus;
        PPU ppu;
        APU apu;
        GUI gui;
        ROM rom;

        bool quit;                  // Quit the NES.
        bool paused;                // Pause the game.
        bool run_frame;             // Run a single frame.
        
        int FPS;                    // The framerate of the NES.

        const uint8_t* key_state;   // For reading keyboard input.

        bool debug_log;             // For logging debug information. Flipped using the INPUT_LOG key.

        NES();
        ~NES();

        uint8_t initialize(string romName);
        void executeFrame();        // Execute an entire frame of instructions.
        void reset();

    private:
        uint32_t total_cycles;      // Counts the total cycles since the start of the program.

        void transferOAM();         // Either read or write 1 byte of data for OAM writing.
        void logDebugInfo();        // Prints debug log information.
};



#endif
