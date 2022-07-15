#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "NES.h"

#define ALLOW_LR_UD false

// Handles all keyboard inputs for the NES.
void handleInput(NES &nes);

#endif
