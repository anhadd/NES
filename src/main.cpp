#include <iostream>
#include <SDL2/SDL.h>

#include <chrono>
#include <thread>

#include "../include/NES.h"
#include "../include/input.h"


using namespace std;
using namespace std::this_thread;
using namespace std::chrono;



int main(int argc, char *argv[])
{
    //set up SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    
    NES nes;

    remove("nes_error.log");
    setbuf(freopen("nes_error.log", "w", stderr), NULL);
    
    time_point<std::chrono::steady_clock> next_frame_time = steady_clock::now();

    SDL_ShowWindow(nes.gui.window);
    if (SHOW_DEBUG) {
        SDL_ShowWindow(nes.gui.pattern_window);
        SDL_ShowWindow(nes.gui.palette_window);
        SDL_ShowWindow(nes.gui.nametable_window);
    }
    printf("Window Opened!\n");

    if (argc < 2) {
        printf("Usage: ./main [ROM FILE PATH]\n");
        return 0;
    }

    if (nes.initialize(argv[1]) != 0) {
        printf("Error: Could not open ROM file!\n");
        return 0;
    }
    printf("Rom Loaded!\n");

    while (!nes.quit) {
        next_frame_time += milliseconds(1000/nes.FPS);
        handleInput(nes);

        if (!nes.paused || nes.run_frame) {
            nes.run_frame = false;
            nes.executeFrame();
        }
        
        sleep_until(next_frame_time);
        SDL_UpdateWindowSurface(nes.gui.window);
    }

    if (nes.rom.mapper->prg_ram_enabled) {
        // Saving to save file.
        printf("Saving game...\n");
        remove(nes.rom.save_path.c_str());
        ofstream save_file(nes.rom.save_path, ios::out | ios::binary);
        if (!save_file.is_open()) {
            printf("Error: Could not save the game: %s\n", strerror(errno));
        }
        else {
            ostream_iterator<uint8_t> file_iterator(save_file);
            copy(nes.rom.PRG_ram.begin(), nes.rom.PRG_ram.end(), file_iterator);
            save_file.close();
        }
    }

    SDL_Quit();
    printf("Finished!\n");
    return 0;
}
