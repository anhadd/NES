#include <iostream>
#include <SDL2/SDL.h>

#include <chrono>
#include <thread>

#include "../include/NES.h"
#include "../include/input.h"


using namespace std;
using namespace std::this_thread;
using namespace std::chrono;



int main(int argc, char *argv[])
{
    //set up SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    
    NES nes;

    remove("nes_error.log");
    setbuf(freopen("nes_error.log", "w", stderr), NULL);
    
    std::time_t next_frame_time = 0;

    SDL_ShowWindow(nes.gui.window);
    if (SHOW_DEBUG) {
        SDL_ShowWindow(nes.gui.pattern_window);
        SDL_ShowWindow(nes.gui.palette_window);
        SDL_ShowWindow(nes.gui.nametable_window);
    }
    printf("Window Opened!\n");

    if (nes.initialize(argv[1]) != 0) {
        printf("Error: Could not open ROM file!\n");
        return 0;
    }
    printf("Rom Loaded!\n");

    while (!nes.quit) {
        next_frame_time = system_clock::to_time_t(system_clock::now() + milliseconds(1000/nes.FPS));
        handleInput(nes);

        if (!nes.paused || nes.run_frame) {
            nes.run_frame = false;
            nes.executeFrame();
        }
        
        sleep_until(system_clock::from_time_t(next_frame_time));
    }

    SDL_Quit();
    printf("Finished!\n");
    return 0;
}
