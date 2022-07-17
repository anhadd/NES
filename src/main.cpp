#include <iostream>
#include <SDL2/SDL.h>

#include <chrono>
#include <thread>
#include <iterator>

#include "../include/NES.h"
#include "../include/input.h"


using namespace std;
using namespace std::this_thread;
using namespace std::chrono;


void saveGame(NES nes) {
    // Saving to save file.
    printf("Saving game...\n");
    // Delete the old save.
    remove(nes.rom.save_path.c_str());
    // Create the new save and write the RAM data to it.
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


int main(int argc, char *argv[]) {
    // Set up SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    
    // Create NES object. Contains everything NES related.
    NES nes;

    // Delete old log file and open a new one at stderr.
    remove("nes_error.log");
    setbuf(freopen("nes_error.log", "w", stderr), NULL);
    
    // Used for keeping track of when the next frame should be shown.
    time_point<std::chrono::steady_clock> next_frame_time = steady_clock::now();

    // Show SDL windows.
    SDL_ShowWindow(nes.gui.window);
    if (SHOW_DEBUG) {
        SDL_ShowWindow(nes.gui.pattern_window);
        SDL_ShowWindow(nes.gui.palette_window);
        SDL_ShowWindow(nes.gui.nametable_window);
    }
    printf("Window Opened!\n");

    // Check whether there has been a ROM file given.
    if (argc < 2) {
        printf("Usage: ./main [ROM FILE PATH]\n");
        return 0;
    }
    // Initialize the NES. Fails if the ROM file could not be found.
    if (nes.initialize(argv[1]) != 0) {
        printf("Error: Could not open ROM file!\n");
        return 0;
    }
    printf("Rom Loaded!\n");

    // Unpause the audio device (start playing sound).
    SDL_PauseAudioDevice(nes.gui.audio_device, 0);

    // Main loop. Keep going until the NES has been asked to quit.
    while (!nes.quit) {
        next_frame_time = steady_clock::now() + milliseconds(1000/nes.FPS);
        handleInput(nes);
        // Execute a frame if the NES is not paused, or if there has been a command to run a single frame.
        if (!nes.paused || nes.run_frame) {
            nes.run_frame = false;
            nes.executeFrame();
        }
        // Wait until the correct frame time to show the new frame.
        sleep_until(next_frame_time);
        SDL_RenderPresent(nes.gui.renderer);
    }

    // Save the game if the ROM supports it.
    if (nes.rom.mapper->prg_ram_enabled) {
        saveGame(nes);
    }

    // Quit SDL and end the program.
    SDL_Quit();
    printf("Finished!\n");
    return 0;
}
