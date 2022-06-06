#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#define GUI_WIDTH 256
#define GUI_HEIGHT 240
#define GUI_SCALE 2



class GUI {
    public:
        // Visual
        SDL_Window *window;
        SDL_Surface *surface;
        SDL_Surface *surface_buff;
        SDL_Rect scaled_screen_rect;

        SDL_Window *pattern_window;
        SDL_Surface *pattern_surface;
        SDL_Surface *pattern_surface_buff;
        SDL_Rect scaled_pattern_rect;

        SDL_Window *palette_window;
        SDL_Surface *palette_surface;
        SDL_Surface *palette_surface_buff;
        SDL_Rect scaled_palette_rect;

        SDL_Window *nametable_window;
        SDL_Surface *nametable_surface;
        SDL_Surface *nametable_surface_buff;
        SDL_Rect scaled_nametable_rect;

        // Audio
        SDL_AudioDeviceID audio_device;
        SDL_AudioSpec audio_spec;

        uint16_t volume;


        SDL_Event sdlevent;

        GUI(int width = GUI_WIDTH, int height = GUI_HEIGHT, int scale = GUI_SCALE);
        ~GUI();

        void createDebugWindows();

    private:
        bool debug_windows_created;
};



#endif
