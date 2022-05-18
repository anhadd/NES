#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#define GUI_WIDTH 256
#define GUI_HEIGHT 240
#define GUI_SCALE 2



class GUI {
    public:
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

        SDL_Event sdlevent;

        bool debug_windows_created;

        GUI(int width = GUI_WIDTH, int height = GUI_HEIGHT, int scale = GUI_SCALE);
        ~GUI();

        void createDebugWindows();
};



#endif
