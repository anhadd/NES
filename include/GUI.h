#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>

#define GUI_WIDTH 256
#define GUI_HEIGHT 240
#define GUI_SCALE 3



class GUI {
    public:
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Texture *texture;
        SDL_Event sdlevent;

        GUI(int width = GUI_WIDTH, int height = GUI_HEIGHT, int scale = GUI_SCALE);
        ~GUI();
};



#endif
