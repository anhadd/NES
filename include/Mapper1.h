#ifndef MAPPER1_H
#define MAPPER1_H

#include <SDL2/SDL.h>
#include <iostream>

#include "Mapper.h"

using namespace std;

#define REG_RESET 0x80

// 4bit0
// -----
// CPPMM
// |||||
// |||++- Mirroring (0: one-screen, lower bank; 1: one-screen, upper bank;
// |||               2: vertical; 3: horizontal)
// |++--- PRG ROM bank mode (0, 1: switch 32 KB at $8000, ignoring low bit of bank number;
// |                         2: fix first bank at $8000 and switch 16 KB bank at $C000;
// |                         3: fix last bank at $C000 and switch 16 KB bank at $8000)
// +----- CHR ROM bank mode (0: switch 8 KB at a time; 1: switch two separate 4 KB banks)
union control_register {
    struct {
        uint8_t mirroring0 : 1;
        uint8_t mirroring1 : 1;
        uint8_t prg_full : 2;
        uint8_t chr_full : 1;
        uint8_t unused : 3;
    };
    uint8_t full;
};


class Mapper1 : public Mapper {
    public:
        Mapper1(uint8_t nPRG, uint8_t nCHR);
        ~Mapper1();

        uint32_t cpuMap(uint16_t address, bool write, uint8_t value = 0x00);
        uint32_t ppuMap(uint16_t address, bool write, uint8_t value = 0x00);
    
    private:
        uint8_t prg_bank0;
        uint8_t prg_bank1;

        uint8_t chr_bank0;
        uint8_t chr_bank1;

        uint8_t load_reg;
        uint8_t load_reg_index;
        union control_register ctrl_reg;
};


#endif
