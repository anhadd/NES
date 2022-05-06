#include <iostream>
#include <SDL2/SDL.h>

#include "../include/NES.h"
#include "../include/input.h"


using namespace std;



int main(int argc, char *argv[])
{
    //set up SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    
    NES nes;

    remove("nes_error.log");
    freopen("nes_error.log", "w", stderr);
    
    bool quit = false;
    int FPS = 20; // TODO: Should be 60 at the end

    SDL_ShowWindow(nes.gui.window);
    printf("Window Opened!\n");

    if (nes.initialize(argv[1]) != 0) {
        printf("Error: Could not open ROM file!\n");
        return 0;
    }
    printf("Rom Loaded!\n");

    while (!quit) {
        quit = handleInput(quit, nes.gui.sdlevent, nes, FPS);
        SDL_Delay(1000/FPS);

        nes.executeFrame();
    }

    SDL_Quit();
    printf("Finished!\n");
    return 0;
}
