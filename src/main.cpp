#include <iostream>
#include <SDL2/SDL.h>

#include "../include/GUI.h"
#include "../include/CPU.h"
#include "../include/input.h"


using namespace std;



// DONE: ADDED 9 AND 0 TO DECREASE / INCREASE SPEED
// DONE: SDL2 DOESNT WORK YET -> THE COMPILER FLAGS WERE WRONG OR SOMETHING
// DONE: FIXED BREAKOUT, APPARENTLY THE ROM WAS BROKEN
// DONE: check 8xy5 and 8xy6? Just try to pass the test_opcode rom. <- WAS WITH SIGNEDNESS AND SHIFTING
int main(int argc, char *argv[])
{
    CPU cpu;
    
    //set up SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    const int height = 240, width = 256, scale = 4;

    GUI gui(height, width, scale);
    
    // SFX sfx;
    // bool play_sound = false;
    
    bool quit = false;
    int FPS = 60;

    SDL_ShowWindow(gui.window);
    cout << "Window Opened!" << endl;

    // TODO: LOAD ROM HERE, FOR NOW JUST LOADS NESTEST
    cpu.loadRom();
    cout << "Rom Loaded!" << endl;

    while (!quit) {
        quit = handleInput(quit, gui.sdlevent, cpu, FPS);
        SDL_Delay(1000/FPS);

        cpu.executeCycle();
    }

    SDL_Quit();
    cout << "Finished!\n";
    return 0;
}
