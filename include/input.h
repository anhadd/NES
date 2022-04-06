#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "NES.h"


bool handleInput(bool quit, SDL_Event sdlevent, NES &nes, int &FPS);


#endif
