#include <iostream>
#include <SDL2/SDL.h>

#include "../include/NES.h"
#include "../include/input.h"


using namespace std;



// DONE: ADDED 9 AND 0 TO DECREASE / INCREASE SPEED
// DONE: SDL2 DOESNT WORK YET -> THE COMPILER FLAGS WERE WRONG OR SOMETHING
// DONE: FIXED BREAKOUT, APPARENTLY THE ROM WAS BROKEN
// DONE: check 8xy5 and 8xy6? Just try to pass the test_opcode rom. <- WAS WITH SIGNEDNESS AND SHIFTING
int main(int argc, char *argv[])
{
    NES nes;

    remove("nes_error.log");
    freopen("nes_error.log", "w", stderr);
    
    //set up SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    const int height = 240, width = 256, scale = 2;
    GUI gui(height, width, scale);
    
    bool quit = false;
    int FPS = 2000;

    SDL_ShowWindow(gui.window);
    printf("Window Opened!\n");

    if (nes.initialize(argv[1]) != 0) {
        fprintf(stderr, "Error: Could not open ROM file!\n");
        return 0;
    }
    while (!quit) {
        quit = handleInput(quit, gui.sdlevent, nes, FPS);
        SDL_Delay(1000/FPS);

        nes.executeCycle();
    }

    SDL_Quit();
    printf("Finished!\n");
    return 0;
}
