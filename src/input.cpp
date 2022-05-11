#include "../include/input.h"


bool handleInput(bool quit, SDL_Event sdlevent, NES &nes, int &FPS) {
    while (SDL_PollEvent(&sdlevent)){
        if (sdlevent.type == SDL_QUIT){
            quit = true;
        }
        else if (sdlevent.type == SDL_KEYDOWN){
            switch (sdlevent.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
            }
        }
        else if (sdlevent.type == SDL_KEYUP){
            // TODO: ADD INPUT SUPPORT !!!
            switch (sdlevent.key.keysym.sym) {
                case INPUT_A:
                    break;
                case INPUT_B:
                    break;
                case INPUT_START:
                    break;
                case INPUT_SELECT:
                    break;
                case INPUT_UP:
                    break;
                case INPUT_DOWN:
                    break;
                case INPUT_LEFT:
                    break;
                case INPUT_RIGHT:
                    break;
                case INPUT_PALETTE:
                    if (nes.ppu.curr_palette >= 7) {
                        nes.ppu.curr_palette = 0;
                    }
                    else {
                        nes.ppu.curr_palette += 1;
                    }
                    break;
                case INPUT_SLOW:
                    if (FPS > 10) {
                        FPS -= 10;
                    }
                    break;
                case INPUT_FAST:
                    FPS += 10;
                    break;
            }
        }
    }

    return quit;
}
