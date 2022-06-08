#include "../include/input.h"


void handleInput(NES &nes) {

    nes.bus.controller[0] = 0x00;
    nes.bus.controller[0] |= nes.key_state[INPUT_A]        << 7;
    nes.bus.controller[0] |= nes.key_state[INPUT_B]        << 6;
    nes.bus.controller[0] |= nes.key_state[INPUT_SELECT]   << 5;
    nes.bus.controller[0] |= nes.key_state[INPUT_START]    << 4;
    nes.bus.controller[0] |= nes.key_state[INPUT_UP]       << 3;
    nes.bus.controller[0] |= nes.key_state[INPUT_DOWN]     << 2;
    nes.bus.controller[0] |= nes.key_state[INPUT_LEFT]     << 1;
    nes.bus.controller[0] |= nes.key_state[INPUT_RIGHT]    << 0;

    while (SDL_PollEvent(&nes.gui.sdlevent)){
        if (nes.gui.sdlevent.type == SDL_QUIT){
            nes.quit = true;
        }
        else if (nes.gui.sdlevent.type == SDL_KEYDOWN){
            switch (nes.gui.sdlevent.key.keysym.sym) {
                case INPUT_QUIT:
                    nes.quit = true;
                    break;
                case INPUT_RESET:
                    nes.reset();
                    break;
                case INPUT_PAUSE:
                    nes.paused = !nes.paused;
                    break;
                case INPUT_LOG:
                    fprintf(stderr, "\n\n       !!! LOG START !!!          \n\n\n");
                    nes.debug_log = !nes.debug_log;
                    break;
                case INPUT_FRAME:
                    nes.run_frame = true;
                    break;
            }
        }
        else if (nes.gui.sdlevent.type == SDL_KEYUP){
            switch (nes.gui.sdlevent.key.keysym.sym) {
                case INPUT_PALETTE:
                    if (nes.ppu.curr_palette >= 7) {
                        nes.ppu.curr_palette = 0;
                    }
                    else {
                        nes.ppu.curr_palette += 1;
                    }
                    break;
                case INPUT_SLOW:
                    if (nes.FPS > 10) {
                        nes.FPS -= 10;
                    }
                    break;
                case INPUT_FAST:
                    nes.FPS += 10;
                    break;
            }
        }
    }
}
