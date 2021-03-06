#include "../include/input.h"


void handleInput(NES &nes) {

    // Read keyboard input into the controller register.
    nes.bus.controller[0] = 0x00;
    nes.bus.controller[0] |= nes.key_state[INPUT_A]             << 7;
    nes.bus.controller[0] |= nes.key_state[INPUT_B]             << 6;
    nes.bus.controller[0] |= nes.key_state[INPUT_SELECT]        << 5;
    nes.bus.controller[0] |= nes.key_state[INPUT_START]         << 4;
    
    // The reason that disabling lr/up is here as a user option is because it is not necessarily part of the NES.
    // It was just physically hard to do on the NES controller, but the NES would normally allow it if it was done.
    // The option is here since it depends on what the user wants the emulator to do, not what the NES actually did, just like showing the debug windows.
    // If pressing left/right or up/down at the same time is allowed.
    if (ALLOW_LR_UD) {
        nes.bus.controller[0] |= nes.key_state[INPUT_UP]        << 3;
        nes.bus.controller[0] |= nes.key_state[INPUT_DOWN]      << 2;
        nes.bus.controller[0] |= nes.key_state[INPUT_LEFT]      << 1;
        nes.bus.controller[0] |= nes.key_state[INPUT_RIGHT]     << 0;
    }
    // Else do not allow lr/ud to be pressed at the same time.
    else {
        nes.bus.controller[0] |= (nes.key_state[INPUT_DOWN]) ? 0 : nes.key_state[INPUT_UP]          << 3;
        nes.bus.controller[0] |= (nes.key_state[INPUT_UP]) ? 0 : nes.key_state[INPUT_DOWN]          << 2;
        nes.bus.controller[0] |= (nes.key_state[INPUT_RIGHT]) ? 0 : nes.key_state[INPUT_LEFT]       << 1;
        nes.bus.controller[0] |= (nes.key_state[INPUT_LEFT]) ? 0 : nes.key_state[INPUT_RIGHT]       << 0;
    }

    // Check if there is a pending SDL event.
    while (SDL_PollEvent(&nes.gui.sdlevent)) {
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
                // The debug log button. Enables/Disables logging NES stuff to file.
                // Disabled since accidentally pressing this button could cause issues.
                // case INPUT_LOG:
                //     fprintf(stderr, "Start Logging:\n\n");
                //     nes.debug_log = !nes.debug_log;
                //     break;
                // Execute a single frame.
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
