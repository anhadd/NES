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
                case SDLK_x:
                    nes.keys[0] = 1;
                    break;
                case SDLK_1:
                    nes.keys[1] = 1;
                    break;
                case SDLK_2:
                    nes.keys[2] = 1;
                    break;
                case SDLK_3:
                    nes.keys[3] = 1;
                    break;
                case SDLK_q:
                    nes.keys[4] = 1;
                    break;
                case SDLK_w:
                    nes.keys[5] = 1;
                    break;
                case SDLK_e:
                    nes.keys[6] = 1;
                    break;
                case SDLK_a:
                    nes.keys[7] = 1;
                    break;
                case SDLK_s:
                    nes.keys[8] = 1;
                    break;
                case SDLK_d:
                    nes.keys[9] = 1;
                    break;
                case SDLK_z:
                    nes.keys[10] = 1;
                    break;
                case SDLK_c:
                    nes.keys[11] = 1;
                    break;
                case SDLK_4:
                    nes.keys[12] = 1;
                    break;
                case SDLK_r:
                    nes.keys[13] = 1;
                    break;
                case SDLK_f:
                    nes.keys[14] = 1;
                    break;
                case SDLK_v:
                    nes.keys[15] = 1;
                    break;
            }
        }
        else if (sdlevent.type == SDL_KEYUP){
            switch (sdlevent.key.keysym.sym) {
                case SDLK_x:
                    nes.keys[0] = 0;
                    break;
                case SDLK_1:
                    nes.keys[1] = 0;
                    break;
                case SDLK_2:
                    nes.keys[2] = 0;
                    break;
                case SDLK_3:
                    nes.keys[3] = 0;
                    break;
                case SDLK_q:
                    nes.keys[4] = 0;
                    break;
                case SDLK_w:
                    nes.keys[5] = 0;
                    break;
                case SDLK_e:
                    nes.keys[6] = 0;
                    break;
                case SDLK_a:
                    nes.keys[7] = 0;
                    break;
                case SDLK_s:
                    nes.keys[8] = 0;
                    break;
                case SDLK_d:
                    nes.keys[9] = 0;
                    break;
                case SDLK_z:
                    nes.keys[10] = 0;
                    break;
                case SDLK_c:
                    nes.keys[11] = 0;
                    break;
                case SDLK_4:
                    nes.keys[12] = 0;
                    break;
                case SDLK_r:
                    nes.keys[13] = 0;
                    break;
                case SDLK_f:
                    nes.keys[14] = 0;
                    break;
                case SDLK_v:
                    nes.keys[15] = 0;
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
