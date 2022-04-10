#include "../include/input.h"


bool handleInput(bool quit, SDL_Event sdlevent, CPU &cpu, int &FPS) {
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
