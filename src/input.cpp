#include "../include/input.h"


bool handleInput(bool quit, SDL_Event sdlevent, NES &nes, int &FPS) {

    nes.bus.controller[0] = 0x00;
    nes.bus.controller[0] |= nes.key_state[INPUT_A]        << 7;
    nes.bus.controller[0] |= nes.key_state[INPUT_B]        << 6;
    nes.bus.controller[0] |= nes.key_state[INPUT_SELECT]   << 5;
    nes.bus.controller[0] |= nes.key_state[INPUT_START]    << 4;
    nes.bus.controller[0] |= nes.key_state[INPUT_UP]       << 3;
    nes.bus.controller[0] |= nes.key_state[INPUT_DOWN]     << 2;
    nes.bus.controller[0] |= nes.key_state[INPUT_LEFT]     << 1;
    nes.bus.controller[0] |= nes.key_state[INPUT_RIGHT]    << 0;

    while (SDL_PollEvent(&sdlevent)){
        if (sdlevent.type == SDL_QUIT){
            quit = true;
        }
        else if (sdlevent.type == SDL_KEYDOWN){
            switch (sdlevent.key.keysym.sym) {
                case INPUT_QUIT:
                    quit = true;
                    break;
            }
        }
        else if (sdlevent.type == SDL_KEYUP){
            switch (sdlevent.key.keysym.sym) {
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
