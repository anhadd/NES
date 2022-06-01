#include "../include/Mapper1.h"





Mapper1::Mapper1(uint8_t nPRG, uint8_t nCHR) {
    // Constructor
    id = 0;
    PRG_banks = nPRG;
    CHR_banks = nCHR;

    prg_bank0 = 0;
    prg_bank1 = 0;

    chr_bank0 = 0;
    chr_bank1 = 0;

    load_reg = 0x00;
    load_reg_index = 0;
    ctrl_reg.full = 0x00;
}


Mapper1::~Mapper1() {
    // Destructor
    
}


uint32_t Mapper1::cpuMap(uint16_t address, bool write, uint8_t value) {
    if (write) {
        if (address >= 0x6000 && address <= 0x7FFF) {
            return address & 0x1FFF;
        } 
        else {
            // 7  bit  0
            // ---- ----
            // Rxxx xxxD
            // |       |
            // |       +- Data bit to be shifted into shift register, LSB first
            // +--------- 1: Reset shift register and write Control with (Control OR $0C),
            //             locking PRG ROM at $C000-$FFFF to the last bank.
            if (value & REG_RESET) {
                ctrl_reg.full |= 0x0C;

                load_reg = 0x00;
                load_reg_index = 0;
            }
            else {
                load_reg >>= 1;
                load_reg |= (value & 0x01) << 4;
                load_reg_index += 1;

                if (load_reg == 5) {
                    switch (address & 0xF000) {
                        case 0x8000:
                            ctrl_reg.full = load_reg & 0x1F;
                            break;
                        case 0xA000:
                            chr_bank0 = load_reg & 0x1F;
                            break;
                        case 0xC000:
                            chr_bank1 = load_reg & 0x1F;
                            break;
                        case 0xE000:
                            if (ctrl_reg.prg_full <= 1) {
                                prg_bank0 = load_reg & 0x1F;
                            }
                            else if (ctrl_reg.prg_full == 2) {
                                prg_bank0 = 0;
                                prg_bank1 = load_reg & 0x1F;
                            }
                            else {
                                prg_bank0 = load_reg & 0x1F;
                                prg_bank1 = 0;
                            }
                            break;
                    }
                    load_reg = 0x00;
                    load_reg_index = 0;
                }
            }
            return address;
        }
    }
    else {
        if (address >= 0x6000 && address <= 0x7FFF) {
            return address & 0x1FFF;
        } 
        else {
            if (ctrl_reg.prg_full <= 1) {
                return prg_bank0 * 0x8000 + (address & 0x7FFF);
            }
            else if (ctrl_reg.prg_full == 2) {
                return prg_bank1 * 0x4000 + (address & 0x7FFF);
            }
            else {
                return prg_bank0 * 0x4000 + (address & 0x7FFF);
            }
        }
    }
}


uint32_t Mapper1::ppuMap(uint16_t address, bool write, uint8_t value) {
    if (write) {
        return address;
    }
    else {
        if (ctrl_reg.chr_full) {
            if (address <= 0x0FFF) {
                return chr_bank0 * 0x1000 + address;
            }
            else {
                return chr_bank1 * 0x1000 + (address & 0x0FFF);
            }
        }
        else {
            return chr_bank0 * 0x2000 + (address & 0x1FFF);
        }
    }
}
