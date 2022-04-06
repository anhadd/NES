#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>


class GUI {
    public:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_Event sdlevent;

        GUI(const int height = 32, const int width = 64, const int scale = 10);
        ~GUI();
};



#endif
