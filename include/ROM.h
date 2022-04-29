#ifndef ROM_H
#define ROM_H

#include <SDL2/SDL.h>
#include <fstream>

using namespace std;

#define ROM_HEADER_SIZE 0x0010
#define PRG_BLOCK_SIZE 0x4000
#define TRAINER_SIZE 512
#define MEMORY_ARRAY_SIZE 0x10000

/*
(Taken from: https://www.nesdev.org/wiki/INES)
An iNES file consists of the following sections, in order:

Header (16 bytes)
Trainer, if present (0 or 512 bytes)
PRG ROM data (16384 * x bytes)
CHR ROM data, if present (8192 * y bytes)
PlayChoice INST-ROM, if present (0 or 8192 bytes)
PlayChoice PROM, if present (16 bytes Data, 16 bytes CounterOut) (this is often missing, see PC10 ROM-Images for details)


The format of the header is as follows:

0-3: Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
4: Size of PRG ROM in 16 KB units
5: Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
6: Flags 6 - Mapper, mirroring, battery, trainer
7: Flags 7 - Mapper, VS/Playchoice, NES 2.0
8: Flags 8 - PRG-RAM size (rarely used extension)
9: Flags 9 - TV system (rarely used extension)
10: Flags 10 - TV system, PRG-RAM presence (unofficial, rarely used extension)
11-15: Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15)
*/

union flags6 {
    struct {
        uint8_t mirroring : 1;              // 0 = horizontal, 1 = vertical.
        uint8_t persistent_memory : 1;
        uint8_t trainer_present : 1;
        uint8_t ignore_mirroring : 1;
        uint8_t mapper_lower : 4;
    };
    uint8_t full;
};

union flags7 {
    struct {
        uint8_t vs_unisystem : 1;
        uint8_t playchoice10 : 1;
        uint8_t nes2 : 2;                   // If == 2, use NES 2.0 format.
        uint8_t mapper_upper : 4;
    };
    uint8_t full;
};

// flags8 is just the prg_ram_size.

union flags9 {
    struct {
        uint8_t tv_system : 1;          // 0 = NTSC, 1 = PAL
        uint8_t reserved_zero : 7;
    };
    uint8_t full;
};

union flags10 {
    struct {
        uint8_t tv_system : 2;          // 0 = NTSC, 2 = PAL, 1 or 3 = dual compatible
        uint8_t unused2 : 2;
        uint8_t prg_ram_present : 1;    // 0 = present, 1 = not present
        uint8_t bus_conflicts : 1;
        uint8_t unused1 : 2;
    };
    uint8_t full;
};

union header {
    struct {
        uint8_t constant[4];
        uint8_t prg_rom_size;       // In 16KB units.
        uint8_t chr_rom_size;       // In 8KB units.
        union flags6 f6;
        union flags7 f7;
        uint8_t prg_ram_size;       // flags8
        union flags9 f9;
        union flags10 f10;
        uint8_t unused[5];
    };
    uint8_t full[16];
};


class ROM {
    public:
        union header h;

        ROM();
        ~ROM();

        bool loadRom(char* romName, uint8_t (&memory)[MEMORY_ARRAY_SIZE]);
};



#endif
