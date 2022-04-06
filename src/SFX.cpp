#include "../include/SFX.h"


SFX::SFX() {
    // Constructor
    SDL_LoadWAV(SFX_PATH, &spec, &buffer, &length);
    device = SDL_OpenAudioDevice(NULL, 0, &spec, NULL, 0);
}


SFX::~SFX() {
    // Destructor
    SDL_CloseAudioDevice(device);
    SDL_FreeWAV(buffer);
}
