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
            switch (sdlevent.key.keysym.sym) {
                case SDLK_p:
                    if (nes.ppu.curr_palette >= 7) {
                        nes.ppu.curr_palette = 0;
                    }
                    else {
                        nes.ppu.curr_palette += 1;
                    }
                    break;
                case SDLK_9:
                    if (FPS > 10) {
                        FPS -= 10;
                    }
                    break;
                case SDLK_0:
                    FPS += 10;
                    break;
            }
        }
    }

    return quit;
}
