#include "../include/Mapper.h"



Mapper::Mapper(uint8_t nPRG, uint8_t nCHR) {
    // Constructor
    id = 0;
    PRG_banks = nPRG;
    CHR_banks = nCHR;

    mirroring = MIRROR_VERTICAL;
    prg_ram_enabled = false;
}


Mapper::~Mapper() {
    // Destructor
    
}
