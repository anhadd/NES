#include "../include/GUI.h"



GUI::GUI(const int height, const int width, const int scale) {
    // Constructor
    window = SDL_CreateWindow("NES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*scale, height*scale, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_RenderSetLogicalSize(renderer, width, height);
    SDL_RenderSetScale(renderer, scale, scale);

    // TODO: CHANGE TO USE WIDTH AND HEIGHT !
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
}


GUI::~GUI() {
    // Destructor
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
}
