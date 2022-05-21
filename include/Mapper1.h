#ifndef MAPPER1_H
#define MAPPER1_H

#include <SDL2/SDL.h>
#include <iostream>


using namespace std;

// TODO: IMPLEMENT MAPPER1
class Mapper1 {
    public:
        uint8_t id;
        uint8_t PRG_banks;
        uint8_t CHR_banks;

        Mapper1();
        ~Mapper1();

        uint16_t cpuMap(uint16_t address);
        uint16_t ppuMap(uint16_t address);
    private:

};


#endif
