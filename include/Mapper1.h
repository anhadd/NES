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
        uint8_t mirroring : 2;
        uint8_t prg_split : 2;
        uint8_t chr_split : 1;
        uint8_t unused : 3;
    };
    uint8_t full;
};

// Mapper1 allows the CHR banks and the PRG banks to be switched.
// It also allows for saving game data on the cartridge.
class Mapper1 : public Mapper {
    public:
        Mapper1(uint8_t nPRG, uint8_t nCHR);
        ~Mapper1();

        uint32_t cpuMap(uint16_t address, bool write, uint8_t value = 0x00);
        uint32_t ppuMap(uint16_t address, bool write, uint8_t value = 0x00);
    
    private:
        uint8_t prg_bank0;          // Stores the number of the PRG bank corresponding to 0x8000-0xBFFF for 16K mode.
                                    // Can also be for the entire 0x8000-0xFFFF range when in 32K mode.
        uint8_t prg_bank1;          // Stores the number of the PRG bank corresponding to 0xC000-0xFFFF for 16K mode.

        uint8_t chr_bank0;          // Stores the number of the CHR bank corresponding to 0x0000-0x0FFF for 4K mode.
                                    // Can also be for the entire 0x0000-0x1FFF range when in 8K mode.
        uint8_t chr_bank1;          // Stores the number of the CHR bank corresponding to 0x0000-0x1FFF for 4K mode.

        uint8_t load_reg;           // Stores the data that should be loaded into a register.
        uint8_t load_reg_index;     // Stores the current index of the bit that has to be loaded into the load_reg.
        union control_register ctrl_reg;    // Control register which contains flags.
};


#endif
