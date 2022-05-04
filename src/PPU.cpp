#include "../include/PPU.h"



PPU::PPU() {
    // Constructor
    cycles = 0;
    scanlines = 0;
    finished = false;
    address_latch = false;

    fill(begin(ppu_patterntable), end(ppu_patterntable), 0);
    fill(begin(ppu_nametable), end(ppu_nametable), 0);
    fill(begin(ppu_palette), end(ppu_palette), 0);

    ppu_ctrl.full = 0x00;
    ppu_mask.full = 0x00;
    ppu_status.full = 0x00;
    oam_addr = 0x00;
    oam_data = 0x00;
    ppu_scroll = 0x00;
    ppu_addr = 0x00;
    ppu_data = 0x00;

    fill(begin(OAM), end(OAM), 0);
    fill(begin(secondary_OAM), end(secondary_OAM), 0);

    data_read_buffer = 0x00;
    curr_palette = 0x00;

    palette_lookup = {
        { 0x00, 84, 84, 84, 255 },
        { 0x01, 0, 30, 116, 255 },
        { 0x02, 8, 16, 144, 255 },
        { 0x03, 48, 0, 136, 255 },
        { 0x04, 68, 0, 100, 255 },
        { 0x05, 92, 0, 48, 255 },
        { 0x06, 84, 4, 0, 255 },
        { 0x07, 60, 24, 0, 255 },
        { 0x08, 32, 42, 0, 255 },
        { 0x09, 8, 58, 0, 255 },
        { 0x0A, 0, 64, 0, 255 },
        { 0x0B, 0, 60, 0, 255 },
        { 0x0C, 0, 50, 60, 255 },
        { 0x0D, 0, 0, 0, 255 },
        { 0x0E, 0, 0, 0, 255 },
        { 0x0F, 0, 0, 0, 255 },

        { 0x10, 152, 150, 152, 255 },
        { 0x11, 8, 76, 196, 255 },
        { 0x12, 48, 50, 236, 255 },
        { 0x13, 92, 30, 228, 255 },
        { 0x14, 136, 20, 176, 255 },
        { 0x15, 160, 20, 100, 255 },
        { 0x16, 152, 34, 32, 255 },
        { 0x17, 120, 60, 0, 255 },
        { 0x18, 84, 90, 0, 255 },
        { 0x19, 40, 114, 0, 255 },
        { 0x1A, 8, 124, 0, 255 },
        { 0x1B, 0, 118, 40, 255 },
        { 0x1C, 0, 102, 120, 255 },
        { 0x1D, 0, 0, 0, 255 },
        { 0x1E, 0, 0, 0, 255 },
        { 0x1F, 0, 0, 0, 255 },

        { 0x20, 236, 238, 236, 255 },
        { 0x21, 76, 154, 236, 255 },
        { 0x22, 120, 124, 236, 255 },
        { 0x23, 176, 98, 236, 255 },
        { 0x24, 228, 84, 236, 255 },
        { 0x25, 236, 88, 180, 255 },
        { 0x26, 236, 106, 100, 255 },
        { 0x27, 212, 136, 32, 255 },
        { 0x28, 160, 170, 0, 255 },
        { 0x29, 116, 196, 0, 255 },
        { 0x2A, 76, 208, 32, 255 },
        { 0x2B, 56, 204, 108, 255 },
        { 0x2C, 56, 180, 204, 255 },
        { 0x2D, 60, 60, 60, 255 },
        { 0x2E, 0, 0, 0, 255 },
        { 0x2F, 0, 0, 0, 255 },

        { 0x30, 236, 238, 236, 255 },
        { 0x31, 168, 204, 236, 255 },
        { 0x32, 188, 188, 236, 255 },

        { 0x33, 212, 178, 236, 255 },
        { 0x34, 236, 174, 236, 255 },
        { 0x35, 236, 174, 212, 255 },

        { 0x36, 236, 180, 176, 255 },
        { 0x37, 228, 196, 144, 255 },
        { 0x38, 204, 210, 120, 255 },

        { 0x39, 180, 222, 120, 255 },
        { 0x3A, 168, 226, 144, 255 },
        { 0x3B, 152, 226, 180, 255 },

        { 0x3C, 160, 214, 228, 255 },
        { 0x3D, 160, 162, 160, 255 },
        { 0x3E, 0, 0, 0, 255 },
        { 0x3F, 0, 0, 0, 255 },
    };
}


PPU::~PPU() {
    // Destructor
    
}

void PPU::reset() {
    cycles = 0;
    scanlines = 0;
    finished = false;

    SDL_RenderClear(gui->renderer);
}

void PPU::passGUI(GUI* nesGUI) {
    gui = nesGUI;
}

void PPU::incrementPPUAddr() {
    if (ppu_ctrl.ppu_increment) {
        ppu_addr += 32;
    }
    else {
        ppu_addr += 1;
    }
    ppu_addr &= 0x3FFF;
}

uint8_t PPU::ppuRead(uint16_t address) {
    if (address <= 0x1FFF) {
        return ppu_patterntable[address];
    }
    else if (address <= 0x3EFF) {
        return ppu_nametable[address & 0x0FFF];
    }
    else {
        return ppu_palette[address & 0x001F];
    }
}

uint8_t PPU::ppuWrite(uint16_t address, uint8_t value) {
    // TODO: THIS DOESNT WORK FOR SOME REASON, FIX IT (Everything gets bg color ??)
    if (address > 0x3FFF) {
        address &= 0x3FFF;
    }

    if (address <= 0x1FFF) {
        ppu_patterntable[address] = value;
    }
    else if (address <= 0x3EFF) {
        ppu_nametable[address & 0x0FFF] = value;
    }
    else {
        ppu_palette[address & 0x001F] = value;
    }
    return 0;
}

uint8_t PPU::readRegister(uint16_t address) {
    // TODO: Deal with PPU registers.
    uint16_t real_address = 0x2000 + (address & 0x0007);
    uint8_t temp;
    switch (real_address) {
        case CONTROL:
        case MASK:
            // No reading allowed.
            break;
        case STATUS:
            // fprintf(stderr, "PPU STATUS: %02x\n", ppu_status.full);

            // TODO: REMOVE THIS SETTING VBLANK TO 1 AFTER TESTING.
            // ppu_status.v_blank = 1;
            temp = ppu_status.full & 0xE0; // TODO: Add   | (data_read_buffer & 0x1F)   for the noise ??
            ppu_status.v_blank = 0;
            address_latch = false;
            return temp;
        case OAM_ADDR:
            // No reading allowed.
            break;
        case OAM_DATA:
            return OAM[oam_addr];
        case SCROLL:
        case PPU_ADDR:
            // No reading allowed.
            break;
        case PPU_DATA:
            if (ppu_addr <= 0x3EFF) {
                // Reading is buffered.
                uint8_t temp_data = data_read_buffer;
                data_read_buffer = ppuRead(ppu_addr);
                incrementPPUAddr();
                return temp_data;
            }
            else {
                // Reading from palettes is instant.
                data_read_buffer = ppuRead(ppu_addr);
                incrementPPUAddr();
                return data_read_buffer;
            }
    }
    return 0x00;
}

uint8_t PPU::writeRegister(uint16_t address, uint8_t value) {
    // TODO: Deal with PPU registers.
    uint16_t real_address = 0x2000 + (address & 0x0007);
    switch (real_address) {
        case CONTROL:
            ppu_ctrl.full = value;
            break;
        case MASK:
            ppu_mask.full = value;
            break;
        case STATUS:
            // No writing allowed.
            break;
        case OAM_ADDR:
            oam_addr = value;
            break;
        case OAM_DATA:
            OAM[oam_addr] = value;
            break;
        case SCROLL:
            // TOOD: IMPLEMENT THIS
            break;
        case PPU_ADDR:
            // TODO: IMPLEMENT THIS
            if (address_latch) {
                // Write to low bytes.
                ppu_addr = (ppu_addr & 0xFF00) | value;
                address_latch = false;
            }
            else {
                // Write to high bytes.
                ppu_addr = (ppu_addr & 0x00FF) | (uint16_t)(value << 8);
                address_latch = true;
            }
            ppu_addr &= 0x3FFF;
            break;
        case PPU_DATA:
            ppuWrite(ppu_addr, value);
            incrementPPUAddr();
            break;
    }
    return 0;
}

uint16_t PPU::getColorIndex(uint8_t palette, uint8_t index) {
    // if ((ppuRead(0x3F00 + ((palette * 4) + index)) & 0x3F) != 0) {
    //     printf("HEX COLOR: %02x\n", ppuRead(0x3F00 + ((palette * 4) + index)) & 0x3F);
    // }
    return ppuRead(0x3F00 + ((palette * 4) + index)) & 0x3F;
}

void PPU::drawPixel(uint16_t x, uint16_t y, uint16_t color_index) {
    curr_color = palette_lookup[color_index];
    SDL_SetRenderDrawColor(gui->renderer, curr_color.r, curr_color.g, curr_color.b, curr_color.a);
    SDL_RenderDrawPoint(gui->renderer, x, y);
}

void PPU::showPatterntablePixel() {
    // For now from: https://emudev.de/nes-emulator/cartridge-loading-pattern-tables-and-ppu-registers/
    // For debugging only.
    if (scanlines >= 0 && scanlines < 256 && cycles >= 0 && cycles < 128) {
        uint16_t adr = (scanlines / 8 * 0x100) + (scanlines % 8) + (cycles / 8) * 0x10;
        uint8_t pixel = ((ppuRead(adr) >> (7-(cycles % 8))) & 1) + ((ppuRead(adr + 8) >> (7-(cycles % 8))) & 1) * 2;
        // TODO: SWITCH THIS FOR THE GET COLOR ONE.
        // drawPixel(cycles, scanlines, (curr_palette * 4) + pixel);
        drawPixel(cycles, scanlines, getColorIndex(curr_palette, pixel));
    }
}

bool PPU::executeCycle() {
    // Execute a single cycle.
    // TODO: IMPLEMENT THIS !!!
    if (scanlines == 241 && cycles == 1) {
        ppu_status.v_blank = 1;
    }
    else if (scanlines == -1 && cycles == 1) {
        ppu_status.v_blank = 0;
    }

    // TODO: Use the correct color, not just a random one.
    // drawPixel(cycles - 1, scanlines, rand() % 0x3F);
    showPatterntablePixel();

    // TODO: Remove this at the end, prints the full ppu_palette for debugging.
    // for (int i = 0; i < 32; i++) {
    //     fprintf(stderr, "%02x ", ppu_palette[i]);
    // }
    // fprintf(stderr, "\n");

    /*  - With rendering disabled (background and sprites disabled in PPUMASK ($2001)), 
            each PPU frame is 341*262=89342 PPU clocks long. There is no skipped clock every other frame.
        - With rendering enabled, each odd PPU frame is one PPU clock shorter than normal. 
            This is done by skipping the first idle tick on the first visible scanline.
    */
    // The PPU renders 262 scanlines per frame. Each scanline lasts for 341 PPU clock cycles
    // Cycles and scanlines go off screen (> width and height) because the remainder is the V-blank period.
    // During V-blank is usually when stuff is updated, because it is not visible.
    cycles += 1;

    if (cycles >= MAX_COLUMNS)
	{
		cycles = 0;
		
        scanlines += 1;
		if (scanlines >= MAX_SCANLINES)
		{
			scanlines = -1;
			finished = true;
            SDL_RenderPresent(gui->renderer);
		}
	}
    
    return 0;
}
