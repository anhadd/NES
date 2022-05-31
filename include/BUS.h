#ifndef BUS_H
#define BUS_H

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include "PPU.h"
#include "ROM.h"

using namespace std;

class BUS {
    public:
        ROM* rom;
        uint8_t memory[0x0800];         // CPU memory without PRG.
        uint8_t controller[2];          // Used for handling input.

        uint8_t oam_page;               // The page that OAM is transfered from.
        uint8_t oam_index;              // The current byte index to transfer to OAM.
        uint8_t oam_data;               // The current byte to transfer to OAM.
        bool oam_writing;               // Set when data is being transfered to OAM.
        bool cpu_synchronized;          // Set when the CPU is synchronized to start transferring OAM data.
        
        BUS();                          // Constructor.
        ~BUS();                         // Destructor.

        void passPPU(PPU* nesPPU);      // Used for receiving the PPU from the NES.
        void passROM(ROM* nesROM);

        void reset();

        uint8_t busReadCPU(uint16_t address);                   // Reading from CPU memory.
        uint8_t busWriteCPU(uint16_t address, uint8_t value);   // Writing to CPU memory.

    private:
        PPU* ppu;                                               // The PPU received from NES.
        uint8_t controller_shift[2];                            // The shift registers for handling input.
};


#endif
