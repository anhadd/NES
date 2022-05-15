#include "../include/GUI.h"



GUI::GUI(int width, int height, int scale) {
    // Constructor
    // window = SDL_CreateWindow("NES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width*scale, height*scale, SDL_WINDOW_SHOWN);
    window = SDL_CreateWindow("NES", 50, 150, width*scale, height*scale, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, width, height);
    SDL_RenderSetScale(renderer, scale, scale);

    // pattern_window = SDL_CreateWindow("Pattern Tables", 575, 150, 128 * 2, 128 * 2 * 2, SDL_WINDOW_SHOWN);
    // pattern_renderer = SDL_CreateRenderer(pattern_window, -1, 0);
    // SDL_RenderSetLogicalSize(pattern_renderer, 128 * 2, 128 * 2 * 2);
    // SDL_RenderSetScale(pattern_renderer, 2, 2);

    // palette_window = SDL_CreateWindow("Palettes", 100, 75, 320, 30, SDL_WINDOW_SHOWN);
    // palette_renderer = SDL_CreateRenderer(palette_window, -1, 0);
    // SDL_RenderSetLogicalSize(palette_renderer, 32, 1);
    // SDL_RenderSetScale(palette_renderer, 10, 30);

    // nametable_window = SDL_CreateWindow("Nametables", 900, 400, 256*2, 256, SDL_WINDOW_SHOWN);
    // nametable_renderer = SDL_CreateRenderer(nametable_window, -1, 0);
    // SDL_RenderSetLogicalSize(nametable_renderer, 256*2, 256);
    // SDL_RenderSetScale(nametable_renderer, 1, 1);

    // texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height);
}


GUI::~GUI() {
    // Destructor
    SDL_DestroyWindow(window);

    SDL_DestroyWindow(pattern_window);
    SDL_DestroyWindow(palette_window);
    SDL_DestroyWindow(nametable_window);
    SDL_DestroyRenderer(renderer);
}


void GUI::createDebugWindows() {
    pattern_window = SDL_CreateWindow("Pattern Tables", 575, 150, 128 * 2, 128 * 2 * 2, SDL_WINDOW_SHOWN);
    pattern_renderer = SDL_CreateRenderer(pattern_window, -1, 0);
    SDL_RenderSetLogicalSize(pattern_renderer, 128 * 2, 128 * 2 * 2);
    SDL_RenderSetScale(pattern_renderer, 2, 2);

    palette_window = SDL_CreateWindow("Palettes", 100, 75, 320, 30, SDL_WINDOW_SHOWN);
    palette_renderer = SDL_CreateRenderer(palette_window, -1, 0);
    SDL_RenderSetLogicalSize(palette_renderer, 32, 1);
    SDL_RenderSetScale(palette_renderer, 10, 30);

    nametable_window = SDL_CreateWindow("Nametables", 900, 400, 256*2, 256, SDL_WINDOW_SHOWN);
    nametable_renderer = SDL_CreateRenderer(nametable_window, -1, 0);
    SDL_RenderSetLogicalSize(nametable_renderer, 256*2, 256);
    SDL_RenderSetScale(nametable_renderer, 1, 1);
}
