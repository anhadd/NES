#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#define GUI_WIDTH 256
#define GUI_HEIGHT 240
#define GUI_SCALE 2



class GUI {
    public:
        // Main NES window.
        SDL_Window *window;
        SDL_Surface *surface;
        SDL_Surface *surface_buff;
        SDL_Rect scaled_screen_rect;

        // Pattern table window.
        SDL_Window *pattern_window;
        SDL_Surface *pattern_surface;
        SDL_Surface *pattern_surface_buff;
        SDL_Rect scaled_pattern_rect;

        // Palette window.
        SDL_Window *palette_window;
        SDL_Surface *palette_surface;
        SDL_Surface *palette_surface_buff;
        SDL_Rect scaled_palette_rect;

        // Nametable window.
        SDL_Window *nametable_window;
        SDL_Surface *nametable_surface;
        SDL_Surface *nametable_surface_buff;
        SDL_Rect scaled_nametable_rect;

        // Event handler for input.
        SDL_Event sdlevent;

        // Audio
        SDL_AudioDeviceID audio_device;
        SDL_AudioSpec audio_spec;

        uint16_t sample;
        uint16_t volume;

        // Constructor / Destructor.
        GUI(int width = GUI_WIDTH, int height = GUI_HEIGHT, int scale = GUI_SCALE);
        ~GUI();

        SDL_AudioCallback getSample();
        // Creates the debug windows. Used when debugging is turned on.
        void createDebugWindows();

    private:
        // Stores whether the debug windows have been created.
        bool debug_windows_created;
};



#endif
