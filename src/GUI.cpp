#include "../include/GUI.h"



GUI::GUI(int width, int height, int scale) {
    // Constructor
    // Visual elements
    window = SDL_CreateWindow("NES", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width*scale, height*scale, SDL_WINDOW_SHOWN);

    surface = SDL_GetWindowSurface(window);
    surface_buff = SDL_CreateRGBSurface(0, width, height, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    scaled_screen_rect.x = 0;
    scaled_screen_rect.y = 0;
    SDL_GetWindowSize(window, &scaled_screen_rect.w, NULL);
    SDL_GetWindowSize(window, NULL, &scaled_screen_rect.h);

    debug_windows_created = false;

    // Audio elements
    SDL_zero(audio_spec);
    audio_spec.freq = 44100;
    audio_spec.format = AUDIO_S16SYS;
    audio_spec.channels = 1;
    audio_spec.samples = 1024;
    audio_spec.callback = NULL;

    audio_device = SDL_OpenAudioDevice(NULL, 0, &audio_spec, NULL, 0);

    volume = 3000;
}


GUI::~GUI() {
    // Destructor
    SDL_DestroyWindow(window);
    SDL_FreeSurface(surface_buff);

    if (debug_windows_created) {
        SDL_DestroyWindow(pattern_window);
        SDL_FreeSurface(pattern_surface_buff);

        SDL_DestroyWindow(palette_window);
        SDL_FreeSurface(palette_surface_buff);

        SDL_DestroyWindow(nametable_window);
        SDL_FreeSurface(nametable_surface_buff);
    }

    SDL_CloseAudioDevice(audio_device);
}


void GUI::createDebugWindows() {
    SDL_SetWindowPosition(window, 50, 150);

    pattern_window = SDL_CreateWindow("Pattern Tables", 575, 150, 128 * 2, 128 * 2 * 2, SDL_WINDOW_SHOWN);
    pattern_surface = SDL_GetWindowSurface(pattern_window);
    pattern_surface_buff = SDL_CreateRGBSurface(0, 128, 128 * 2, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    scaled_pattern_rect.x = 0;
    scaled_pattern_rect.y = 0;
    SDL_GetWindowSize(pattern_window, &scaled_pattern_rect.w, NULL);
    SDL_GetWindowSize(pattern_window, NULL, &scaled_pattern_rect.h);

    palette_window = SDL_CreateWindow("Palettes", 100, 75, 32 * 10, 1 * 30, SDL_WINDOW_SHOWN);
    palette_surface = SDL_GetWindowSurface(palette_window);
    palette_surface_buff = SDL_CreateRGBSurface(0, 32, 1, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    scaled_palette_rect.x = 0;
    scaled_palette_rect.y = 0;
    SDL_GetWindowSize(palette_window, &scaled_palette_rect.w, NULL);
    SDL_GetWindowSize(palette_window, NULL, &scaled_palette_rect.h);

    nametable_window = SDL_CreateWindow("Nametables", 900, 400, 256 * 2, 256, SDL_WINDOW_SHOWN);
    nametable_surface = SDL_GetWindowSurface(nametable_window);
    nametable_surface_buff = SDL_CreateRGBSurface(0, 256 * 2, 256, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    scaled_nametable_rect.x = 0;
    scaled_nametable_rect.y = 0;
    SDL_GetWindowSize(nametable_window, &scaled_nametable_rect.w, NULL);
    SDL_GetWindowSize(nametable_window, NULL, &scaled_nametable_rect.h);

    debug_windows_created = true;
}
