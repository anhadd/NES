#ifndef SFX_H
#define SFX_H

#include <SDL2/SDL.h>


#define SFX_PATH "./sound.wav"


class SFX {
    public:
        uint32_t length;
        uint8_t *buffer;
        SDL_AudioSpec spec;
        SDL_AudioDeviceID device;

        SFX();
        ~SFX();
};



#endif
