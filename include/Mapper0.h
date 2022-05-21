#ifndef MAPPER0_H
#define MAPPER0_H

#include <SDL2/SDL.h>
#include <iostream>


using namespace std;

class Mapper0 {
    public:
        uint8_t id;
        uint8_t PRG_banks;
        uint8_t CHR_banks;

        Mapper0();
        ~Mapper0();

        uint16_t cpuMap(uint16_t address);
        uint16_t ppuMap(uint16_t address);
    private:

};


#endif
