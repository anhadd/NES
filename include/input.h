#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "CPU.h"


bool handleInput(bool quit, SDL_Event sdlevent, CPU &cpu, int &FPS);


#endif
