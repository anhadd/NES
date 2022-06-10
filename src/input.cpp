#include "../include/input.h"


void handleInput(NES &nes) {

    // Read keyboard input into the controller register.
    nes.bus.controller[0] = 0x00;
    nes.bus.controller[0] |= nes.key_state[INPUT_A]        << 7;
    nes.bus.controller[0] |= nes.key_state[INPUT_B]        << 6;
    nes.bus.controller[0] |= nes.key_state[INPUT_SELECT]   << 5;
    nes.bus.controller[0] |= nes.key_state[INPUT_START]    << 4;
    nes.bus.controller[0] |= nes.key_state[INPUT_UP]       << 3;
    nes.bus.controller[0] |= nes.key_state[INPUT_DOWN]     << 2;
    nes.bus.controller[0] |= nes.key_state[INPUT_LEFT]     << 1;
    nes.bus.controller[0] |= nes.key_state[INPUT_RIGHT]    << 0;

    // Check if there is a pending SDL event.
    while (SDL_PollEvent(&nes.gui.sdlevent)){
        // Quit whether the sdlevent is of type SDL_QUIT (For example when CTRL+C is pressed in the terminal).
        if (nes.gui.sdlevent.type == SDL_QUIT){
            nes.quit = true;
        }
        // If a button is pressed check what to do depending on the button.
        else if (nes.gui.sdlevent.type == SDL_KEYDOWN){
            switch (nes.gui.sdlevent.key.keysym.sym) {
                // Quit.
                case INPUT_QUIT:
                    nes.quit = true;
                    break;
                // Reset.
                case INPUT_RESET:
                    nes.reset();
                    break;
                // Pause.
                case INPUT_PAUSE:
                    nes.paused = !nes.paused;
                    break;
                // The debug log button. Starts logging NES stuff to file.
                // case INPUT_LOG:
                //     fprintf(stderr, "Start Logging:\n\n");
                //     nes.debug_log = !nes.debug_log;
                //     break;
                // Executes a single frame.
                case INPUT_FRAME:
                    nes.run_frame = true;
                    break;
            }
        }
        // If a button is released do something depending on the button.
        else if (nes.gui.sdlevent.type == SDL_KEYUP){
            switch (nes.gui.sdlevent.key.keysym.sym) {
                // Change palette.
                case INPUT_PALETTE:
                    if (nes.ppu.curr_palette >= 7) {
                        nes.ppu.curr_palette = 0;
                    }
                    else {
                        nes.ppu.curr_palette += 1;
                    }
                    break;
                // Decrease framerate.
                case INPUT_SLOW:
                    if (nes.FPS > 10) {
                        nes.FPS -= 10;
                    }
                    else  if (nes.FPS > 1) {
                        nes.FPS -= 1;
                    }
                    printf("FPS: %u\n", nes.FPS);
                    break;
                // Increase framerate.
                case INPUT_FAST:
                    if (nes.FPS < 10) {
                        nes.FPS += 1;
                    }
                    else {
                        nes.FPS += 10;
                    }
                    printf("FPS: %u\n", nes.FPS);
                    break;
            }
        }
    }
}
