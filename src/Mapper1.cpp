#include "../include/Mapper1.h"





Mapper1::Mapper1(uint8_t nPRG, uint8_t nCHR) {
    // Constructor
    id = 1;
    PRG_banks = nPRG;
    CHR_banks = nCHR;

    prg_bank0 = 0;
    prg_bank1 = nPRG - 1;

    chr_bank0 = 0;
    chr_bank1 = 0;

    load_reg = 0x00;
    load_reg_index = 0;
    ctrl_reg.full = 0x0C;

    // PRG RAM is enabled by default on Mapper1.
    prg_ram_enabled = true;
}


Mapper1::~Mapper1() {
    // Destructor
}


uint32_t Mapper1::cpuMap(uint16_t address, bool write, uint8_t value) {
    if (write) {
        // Writes to PRG RAM are normal.
        if (address >= 0x6000 && address <= 0x7FFF) {
            return address & 0x1FFF;
        }
        // Writes to PRG ROM change the mapper's internal variables (selected bank, registers, etc).
        else {
            //      7  bit  0
            //      ---- ----
            //      Rxxx xxxD
            //      |       |
            //      |       +- Data bit to be shifted into shift register, LSB first
            //      +--------- 1: Reset shift register and write Control with (Control OR $0C),
            //                    locking PRG ROM at $C000-$FFFF to the last bank.
            // If the reset bit is set in the value, reset the load register and do control register OR 0x0C.
            if (value & REG_RESET) {
                ctrl_reg.full |= 0x0C;

                load_reg = 0x00;
                load_reg_index = 0;
            }
            // Else load a new bit into the load register.
            else {
                load_reg >>= 1;
                load_reg |= (value & 0x01) << 4;
                load_reg_index += 1;
                // If the load register is full, write its value to a register depending on the given address.
                if (load_reg_index == 5) {
                    switch (address & 0xF000) {
                        case 0x8000:
                        case 0x9000:
                            // Set the control register to the load register.
                            ctrl_reg.full = load_reg & 0x1F;
                            // Change the mirroring mode depending on the new value in the ctrl_reg.
                            switch (ctrl_reg.mirroring) {
                                case 0:	
                                    mirroring = MIRROR_SINGLE_LOWER;
                                    break;
                                case 1: 
                                    mirroring = MIRROR_SINGLE_UPPER;
                                    break;
                                case 2: 
                                    mirroring = MIRROR_VERTICAL;
                                    break;
                                case 3:	
                                    mirroring = MIRROR_HORIZONTAL;
                                    break;
                            }
                            break;
                        case 0xA000:
                        case 0xB000:
                            // Set chr_bank0 to the load register.
                            if (ctrl_reg.chr_split) {
                                // 4KB switchable CHR mode.
                                chr_bank0 = load_reg & 0x1F;
                            }
                            else {
                                // 8KB switchable CHR mode.
                                // Divide the value by 2 if in 8KB CHR bank mode.
                                chr_bank0 = (load_reg & 0x1E) >> 1;
                            }
                            break;
                        case 0xC000:
                        case 0xD000:
                            // Set chr_bank1 to the load register.
                            // Ignored in 8KB switchable CHR mode.
                            chr_bank1 = load_reg & 0x1F;
                            break;
                        case 0xE000:
                        case 0xF000:
                            // Set prg_bank to the load register depending on the way the PRG ROM area is set up.
                            if (ctrl_reg.prg_split <= 1) {
                                // 32KB mode. Divide load register value by 2.
                                prg_bank0 = (load_reg & 0x0E) >> 1;
                            }
                            else if (ctrl_reg.prg_split == 2) {
                                // 16KB mode. Fix 0 and switch 1.
                                prg_bank0 = 0;
                                prg_bank1 = load_reg & 0x0F;
                            }
                            else {
                                // 16KB mode. Switch 0 and fix 1.
                                prg_bank0 = load_reg & 0x0F;
                                prg_bank1 = PRG_banks - 1;
                            }
                            prg_ram_enabled = (load_reg & 0x10) == 0;
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
        // PRG RAM.
        if (address >= 0x6000 && address <= 0x7FFF) {
            return address & 0x1FFF;
        } 
        // PRG ROM.
        else {
            if (ctrl_reg.prg_split >= 2) {
                // 16KB switchable and 16KB fixed PRG.
                if (address >= 0x8000 && address <= 0xBFFF) {
                    return prg_bank0 * 0x4000 + (address & 0x3FFF);
                }
                else {
                    return prg_bank1 * 0x4000 + (address & 0x3FFF);
                }
            }
            else {
                // 32KB switchable PRG.
                return prg_bank0 * 0x8000 + (address & 0x7FFF);
            }
        }
    }
}


uint32_t Mapper1::ppuMap(uint16_t address, bool write, uint8_t value) {
    // Read from the correct CHR bank depending on the way it is split.
    if (ctrl_reg.chr_split) {
        // 2x 4KB switchable CHR.
        if (address <= 0x0FFF) {
            return chr_bank0 * 0x1000 + (address & 0x0FFF);
        }
        else {
            return chr_bank1 * 0x1000 + (address & 0x0FFF);
        }
    }
    else {
        // 8KB switchable CHR.
        return chr_bank0 * 0x2000 + (address & 0x1FFF);
    }
}
