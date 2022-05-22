#include "../include/PPU.h"





// TODO: Check if supporting 8x16 sprites is necessary.
    // NECESSARY FOR SOME POPULAR ROMS, SO DO IT.

PPU::PPU() {
    // Constructor
    total_frames = 0;
    cycles = 0;
    scanlines = 0;
    finished = false;
    address_latch = false;
    odd_frame = false;

    fill_n(&ppu_nametable[0][0], 2 * 0x0400, 0);
    fill(begin(ppu_palette), end(ppu_palette), 0);

    ppu_ctrl.full = 0x00;
    ppu_mask.full = 0x00;
    ppu_status.full = 0x00;
    oam_addr = 0x00;
    oam_data = 0x00;
    ppu_scroll = 0x00;

    ppu_addr.full = 0x0000;
    ppu_buff.full = 0x0000;
    fine_x = 0x00;

    ppu_data = 0x00;

    memset(sprite_OAM, 0, sizeof(sprite_OAM));
    OAM = (uint8_t*)sprite_OAM;
    memset(sprite_secondary_OAM, 0, sizeof(sprite_secondary_OAM));
    secondary_OAM = (uint8_t*)sprite_secondary_OAM;
    secondary_OAM_index = 0x00;

    fill(begin(sprite_shifter_high), end(sprite_shifter_high), 0);
    fill(begin(sprite_shifter_low), end(sprite_shifter_low), 0);

    sprite_zero = false;

    data_read_buffer = 0x00;
    curr_palette = 0x00;

    // Background rendering variables
    bg_shifter_high = 0x0000;
    bg_shifter_low = 0x0000;
    att_shifter_high = 0x00;
    att_shifter_low = 0x00;

    bg_nametable = 0x00;
    bg_attribute = 0x00;
    bg_low = 0x00;
    bg_high = 0x00;
    
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
    total_frames = 0;
    cycles = 0;
    scanlines = 0;
    finished = false;
    address_latch = false;
    odd_frame = false;

    fill_n(&ppu_nametable[0][0], 2 * 0x0400, 0);
    fill(begin(ppu_palette), end(ppu_palette), 0);

    ppu_ctrl.full = 0x00;
    ppu_mask.full = 0x00;
    ppu_status.full = 0x00;
    oam_addr = 0x00;
    oam_data = 0x00;
    ppu_scroll = 0x00;

    ppu_addr.full = 0x0000;
    ppu_buff.full = 0x0000;
    fine_x = 0x00;

    ppu_data = 0x00;

    memset(sprite_OAM, 0, sizeof(sprite_OAM));
    memset(sprite_secondary_OAM, 0, sizeof(sprite_secondary_OAM));

    data_read_buffer = 0x00;
    curr_palette = 0x00;

    // Background rendering variables
    bg_shifter_high = 0x0000;
    bg_shifter_low = 0x0000;
    att_shifter_high = 0x00;
    att_shifter_low = 0x00;

    bg_nametable = 0x00;
    bg_attribute = 0x00;
    bg_low = 0x00;
    bg_high = 0x00;

    SDL_FillRect(gui->surface, NULL, 0x000000);

    if (show_debug) {
        SDL_FillRect(gui->pattern_surface, NULL, 0x000000);
        SDL_FillRect(gui->palette_surface, NULL, 0x000000);
        SDL_FillRect(gui->nametable_surface, NULL, 0x000000);
    }
}

void PPU::passGUI(GUI* nesGUI) {
    gui = nesGUI;
}

void PPU::passROM(ROM* nesROM) {
    rom = nesROM;
}

void PPU::incrementPPUAddr() {
    if (ppu_ctrl.ppu_increment) {
        ppu_addr.full += 32;
    }
    else {
        ppu_addr.full += 1;
    }
}

uint8_t PPU::ppuRead(uint16_t address) {
    address &= 0x3FFF;

    if (address <= 0x1FFF) {
        return rom->CHR_memory[rom->mapper->ppuMap(address)];
    }
    else if (address <= 0x3EFF) {
        address &= 0x0FFF;
        if (vertical_mirorring) {
            if ((address >= 0 && address < 0x0400) || (address >= 0x0800 && address < 0x0C00)) {
                return ppu_nametable[0][address & 0x03FF];
            }
            else {
                return ppu_nametable[1][address & 0x03FF];
            }
        }
        else {
            if (address >= 0 && address < 0x0800) {
                return ppu_nametable[0][address & 0x03FF];
            }
            else {
                return ppu_nametable[1][address & 0x03FF];
            }
        }
    }
    else {
        address &= 0x001F;

        if (address == 0x0010) address = 0x0000;
        else if (address == 0x0014) address = 0x0004;
        else if (address == 0x0018) address = 0x0008;
        else if (address == 0x001C) address = 0x000C;
    
        return ppu_palette[address];
    }
}

uint8_t PPU::ppuWrite(uint16_t address, uint8_t value) {
    address &= 0x3FFF;

    if (address <= 0x1FFF) {
        rom->CHR_memory[rom->mapper->ppuMap(address)] = value;
    }
    else if (address <= 0x3EFF) {
        address &= 0x0FFF;
        if (vertical_mirorring) {
            if ((address >= 0x0000 && address < 0x0400) || (address >= 0x0800 && address < 0x0C00)) {
                ppu_nametable[0][address & 0x03FF] = value;
            }
            else {
                ppu_nametable[1][address & 0x03FF] = value;
            }
        }
        else {
            if (address >= 0x0000 && address < 0x0800) {
                ppu_nametable[0][address & 0x03FF] = value;
            }
            else {
                ppu_nametable[1][address & 0x03FF] = value;
            }
        }
    }
    else {
        address &= 0x001F;
        if (address == 0x0010) address = 0x0000;
        else if (address == 0x0014) address = 0x0004;
        else if (address == 0x0018) address = 0x0008;
        else if (address == 0x001C) address = 0x000C;

        ppu_palette[address] = value;
    }
    return 0;
}

uint8_t PPU::readRegister(uint16_t address) {
    uint16_t real_address = 0x2000 + (address & 0x0007);
    uint8_t temp;
    switch (real_address) {
        case CONTROL:
        case MASK:
            // No reading allowed.
            break;
        case STATUS:
            // TODO: REMOVE THIS SETTING VBLANK TO 1 AFTER FINISHING TESTING.
            // ppu_status.v_blank = 1;
            // TODO: Maybe add "| (data_read_buffer & 0x1F)"  for the noise stuff ??
            temp = ppu_status.full & 0xE0;
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
            if (ppu_addr.full <= 0x3EFF) {
                // Reading is buffered.
                uint8_t temp_data = data_read_buffer;
                data_read_buffer = ppuRead(ppu_addr.full);
                incrementPPUAddr();
                return temp_data;
            }
            else {
                // Reading from palettes is instant.
                // Buffer gets set to value in VRAM, palette value is returned.
                data_read_buffer = ppuRead(ppu_addr.full - 0x1000);
                incrementPPUAddr();
                return ppuRead(ppu_addr.full);
            }
    }
    return 0x00;
}

uint8_t PPU::writeRegister(uint16_t address, uint8_t value) {
    uint16_t real_address = 0x2000 + (address & 0x0007);
    switch (real_address) {
        case CONTROL:
            ppu_ctrl.full = value;
            ppu_buff.nametable_x = ppu_ctrl.nametable_x;
            ppu_buff.nametable_y = ppu_ctrl.nametable_y;
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
            if (address_latch) {
                // Get the fine_y and coarse_y values.
                ppu_buff.coarse_y = value / 0x08;
                ppu_buff.fine_y = value & 0x07;
                address_latch = false;
            }
            else {
                // Get the fine_x and coarse_x values.
                ppu_buff.coarse_x = value / 0x08;
                fine_x = value & 0x07;
                address_latch = true;
            }
            // ppu_buff.full &= 0x3FFF;
            break;
        case PPU_ADDR:
            if (address_latch) {
                // Write to low bytes.
                ppu_buff.full = (ppu_buff.full & 0xFF00) | value;
                ppu_addr.full = ppu_buff.full; // TODO: Check if this is necessary:  & 0x3FFF
                address_latch = false;
            }
            else {
                // Write to high bytes.
                ppu_buff.full = (ppu_buff.full & 0x00FF) | (uint16_t)(value << 8);
                address_latch = true;
            }
            // ppu_buff.full &= 0x3FFF;
            break;
        case PPU_DATA:
            ppuWrite(ppu_addr.full, value);
            incrementPPUAddr();
            break;
    }
    return 0;
}

uint16_t PPU::getColorIndex(uint8_t palette, uint8_t index) {
    return ppuRead(0x3F00 + ((palette * 4) + index)) & 0x3F;
}

void PPU::drawPixelOnSurface(SDL_Surface *surface, uint16_t x, uint16_t y, uint16_t color_index) {
    curr_color = palette_lookup[color_index];

    uint32_t* target_pixel = (uint32_t*) ((uint8_t*) surface->pixels
                                    + (y * surface->pitch)
                                    + (x * surface->format->BytesPerPixel));
    
    *target_pixel = curr_color.a << 24
                | curr_color.r << 16
                | curr_color.g << 8
                | curr_color.b;
}

void PPU::showPatterntablePixel() {
    // For debugging only.
    
    if (total_frames % 15 == 0) {
        // Show the palette memory colors.
        if (scanlines == 0 && cycles >= 0 && cycles < 32) {
            drawPixelOnSurface(gui->palette_surface_buff, cycles, scanlines, ppuRead(0x3F00 + cycles) & 0x3F);
        }

        if (scanlines == 256 && cycles == 0) {
            // Show the pattern tables.
            for (int sc = 0; sc < 256; sc++) {
                for (int cy = 0; cy < 128; cy++) {
                    uint16_t adr = ((sc / 8) * 0x0100) + (sc % 8) + (cy / 8) * 0x10;
                    uint8_t pixel = ((ppuRead(adr) >> (7-(cy % 8))) & 0x01) + ((ppuRead(adr + 8) >> (7-(cy % 8))) & 0x01) * 2;
                    drawPixelOnSurface(gui->pattern_surface_buff, cy, sc, getColorIndex(curr_palette, pixel));
                }
            }
            
            // Show the nametables.
            for (int i = 0; i < 32; i++) {
                for (int j = 0; j < 32; j++) {
                    uint8_t pattern_id = 0x00;
                    uint8_t pattern_id2 = 0x00;
                    if (vertical_mirorring) {
                        pattern_id = ppuRead(0x2000 + (i*32 + j));
                        pattern_id2 = ppuRead(0x2000 + 0x400 + (i*32 + j));
                    }
                    else {
                        pattern_id = ppuRead(0x2000 + (i*32 + j));
                        pattern_id2 = ppuRead(0x2000 + 0x800 + (i*32 + j));
                    }

                    for (int k = 0; k < 8; k++) {
                        for (int l = 0; l < 8; l++) {
                            // 1 Tile = 16 Bytes.
                            uint16_t adr = (ppu_ctrl.bgr_addr * 0x1000) + (pattern_id * 16) + k;
                            uint8_t pixel = ((ppuRead(adr) >> (7-l)) & 0x01) + ((ppuRead(adr + 8) >> (7-l)) & 0x01) * 2;
                            drawPixelOnSurface(gui->nametable_surface_buff, j*8+l, i*8+k, getColorIndex(curr_palette, pixel));

                            uint16_t adr2 = (ppu_ctrl.bgr_addr * 0x1000) + (pattern_id2 * 16) + k;
                            uint8_t pixel2 = ((ppuRead(adr2) >> (7-l)) & 0x01) + ((ppuRead(adr2 + 8) >> (7-l)) & 0x01) * 2;
                            drawPixelOnSurface(gui->nametable_surface_buff, j*8+l + 256, i*8+k, getColorIndex(curr_palette, pixel2));
                        }
                    }
                }
            }
            SDL_BlitScaled(gui->pattern_surface_buff, NULL, gui->pattern_surface, &gui->scaled_pattern_rect);
            SDL_UpdateWindowSurface(gui->pattern_window);
            SDL_BlitScaled(gui->palette_surface_buff, NULL, gui->palette_surface, &gui->scaled_palette_rect);
            SDL_UpdateWindowSurface(gui->palette_window);
            SDL_BlitScaled(gui->nametable_surface_buff, NULL, gui->nametable_surface, &gui->scaled_nametable_rect);
            SDL_UpdateWindowSurface(gui->nametable_window);
        }
    }
}

void PPU::loadShifters() {
    if (ppu_mask.showbg) {
        bg_shifter_high = (bg_shifter_high & 0xFF00) | bg_high;
        bg_shifter_low = (bg_shifter_low & 0xFF00) | bg_low;

        att_shifter_high = (att_shifter_high & 0xFF00) | (((bg_attribute & 0x02) >> 1) * 0xFF);
        att_shifter_low = (att_shifter_low & 0xFF00) | ((bg_attribute & 0x01) * 0xFF);
    }
}

void PPU::updateShifters() {
    if (ppu_mask.showbg) {
        bg_shifter_high <<= 1;
        bg_shifter_low <<= 1;
        att_shifter_high <<= 1;
        att_shifter_low <<= 1;
    }

    if (ppu_mask.showsprites) {
        for (int i = 0; i < 8; i++) {
            if ((cycles - 2) >= sprite_secondary_OAM[i].x && (cycles - 2) < sprite_secondary_OAM[i].x + 8) {
                sprite_shifter_high[i] <<= 1;
                sprite_shifter_low[i] <<= 1;
            }
        }
    }
}

uint8_t PPU::flipByte(uint8_t byte) {
    byte = (byte & 0b11110000) >> 4 | (byte & 0b00001111) << 4;
    byte = (byte & 0b11001100) >> 2 | (byte & 0b00110011) << 2;
    byte = (byte & 0b10101010) >> 1 | (byte & 0b01010101) << 1;
    return byte;
}

bool PPU::executeCycle() {
    // Execute a single cycle.
    /*  
        - With rendering disabled (background and sprites disabled in PPUMASK ($2001)), 
            each PPU frame is 341*262=89342 PPU clocks long. There is no skipped clock every other frame.
        - With rendering enabled, each odd PPU frame is one PPU clock shorter than normal. 
            This is done by skipping the first idle tick on the first visible scanline.
        - The PPU renders 262 scanlines per frame. Each scanline lasts for 341 PPU clock cycles
        - Cycles and scanlines go off screen (> width and height) because the remainder is the V-blank period.
        - During V-blank is usually when stuff is updated, because it is not visible.
    */

    if (scanlines >= -1 && scanlines <= 239) {
        if (scanlines == -1 && cycles == 1) {
            ppu_status.v_blank = 0;
            ppu_status.sprite_overflow = 0;
            ppu_status.sprite_zerohit = 0;
        }
        else if (scanlines == 0 && cycles == 0 && odd_frame) {
            cycles += 1;
        }

        // if ((cycles >= 1 && cycles <= 257) || (cycles >= 321 && cycles <= 336)) {
        if ((cycles >= 1 && cycles <= 256) || (cycles >= 321 && cycles <= 336)) {
            uint16_t byte_addr = 0x0000;

            updateShifters();

            switch ((cycles - 1) % 8) {
                case 0:
                    loadShifters();
                    bg_nametable = ppuRead(0x2000 + (ppu_addr.full & 0x0FFF));
                    break;
                case 2:
                    /*  Gets the attribute byte.
                        The attribute byte is then split into 4 parts: 04 03 02 01
                        Each part corresponds to one quarter of the tile in the following way:
                            |--------|--------|
                            |-- 01 --|-- 02 --|
                            |--------|--------|
                            |=================|
                            |--------|--------|
                            |-- 03 --|-- 04 --|
                            |--------|--------|
                    */
                    byte_addr = 0x23C0
                            + (ppu_addr.nametable_y * 0x0800)
                            + (ppu_addr.nametable_x * 0x0400)
                            + ((ppu_addr.coarse_y / 4) * 0x08)
                            + (ppu_addr.coarse_x / 4);
                    bg_attribute = ppuRead(byte_addr);
                    if ((ppu_addr.coarse_y % 4) >= 2) {
                        // Top half of attribute 2x2 tile.
                        // Shifts the bg_attribute 4 bits to the right to get there.
                        bg_attribute >>= 4;
                    }
                    if (ppu_addr.coarse_x % 4 >= 2) {
                        // Left half of attribute 2x2 tile.
                        // Shifts the bg_attribute 2 bits to the right to get there.
                        bg_attribute >>= 2;
                    }
                    bg_attribute &= 0x03;
                    break;
                case 4:
                    // uint16_t adr = 0x0000 + (pattern_id * 16) + k;
                    byte_addr = (ppu_ctrl.bgr_addr * 0x1000)
                            + (bg_nametable * 0x10)
                            + ppu_addr.fine_y;
                    bg_low = ppuRead(byte_addr);
                    break;
                case 6:
                    byte_addr = (ppu_ctrl.bgr_addr * 0x1000)
                            + (bg_nametable * 0x10)
                            + ppu_addr.fine_y 
                            + 8;
                    bg_high = ppuRead(byte_addr);
                    break;
                case 7:
                    if (ppu_mask.showbg || ppu_mask.showsprites) {
                        if (ppu_addr.coarse_x == 31) {
                            ppu_addr.coarse_x = 0;
                            ppu_addr.nametable_x ^= 1;
                        }
                        else {
                            ppu_addr.coarse_x += 1;
                        }
                    }
                    break;
            }
        }
        
        if (cycles == 256) {
            if (ppu_mask.showbg || ppu_mask.showsprites) {
                if (ppu_addr.fine_y == 7) {
                    ppu_addr.fine_y = 0;
                    if (ppu_addr.coarse_y == 29) {
                        ppu_addr.coarse_y = 0;
                        ppu_addr.nametable_y ^= 1;
                    }
                    else {
                        ppu_addr.coarse_y += 1;
                    }
                }
                else {
                    ppu_addr.fine_y += 1;
                }
            }
        }
        else if (cycles == 257) {
            if (ppu_mask.showbg || ppu_mask.showsprites) {
                ppu_addr.nametable_x = ppu_buff.nametable_x;
			    ppu_addr.coarse_x = ppu_buff.coarse_x;
            }
        }
        else if (cycles == 337 || cycles == 339) {
            bg_nametable = ppuRead(0x2000 | (ppu_addr.full & 0x0FFF));
        }
        else if (scanlines == -1 && cycles >= 280 && cycles <= 304) {
            if (ppu_mask.showbg || ppu_mask.showsprites) {
                ppu_addr.nametable_y = ppu_buff.nametable_y;
                ppu_addr.coarse_y = ppu_buff.coarse_y;
                ppu_addr.fine_y = ppu_buff.fine_y;
            }
        }




        /* 
            ===============================================================================
            ================================== SPRITES ====================================
            ===============================================================================

            Each scanline, the PPU reads the spritelist (that is, Object Attribute Memory) to see which to draw:

            - First, it clears the list of sprites to draw.
            - Second, it reads through OAM, checking which sprites will be on this scanline. It chooses the first eight it finds that do.
            - Third, if eight sprites were found, it checks (in a wrongly-implemented fashion) for further sprites on the scanline to see if the sprite overflow flag should be set.
            - Fourth, using the details for the eight (or fewer) sprites chosen, it determines which pixels each has on the scanline and where to draw them.

            (from: https://www.nesdev.org/wiki/PPU_sprite_evaluation)
        */
        if (scanlines != -1) {
            if (cycles == 257) {
                memset(sprite_secondary_OAM, 0xFF, sizeof(sprite_secondary_OAM));

                secondary_OAM_index = 0;
                fill(begin(sprite_shifter_high), end(sprite_shifter_high), 0);
                fill(begin(sprite_shifter_low), end(sprite_shifter_low), 0);

                sprite_zero = false;

                for (int i = 0; i < 64; i++) {
                    int16_t distance = scanlines - sprite_OAM[i].y;
                    if (distance >= 0 && distance < 8) {
                        if (secondary_OAM_index < 8) {
                            if (i == 0) {
                                sprite_zero = true;
                            }

                            sprite_secondary_OAM[secondary_OAM_index] = sprite_OAM[i];
                            secondary_OAM_index += 1;
                        }
                        else {
                            ppu_status.sprite_overflow = 1;
                            break;
                        }
                    }
                }
            }
            else if (cycles == 321) {
                uint8_t sprite_low = 0x00;
                uint8_t sprite_high = 0x00;

                uint16_t sprite_addr = 0x0000;

                for (int i = 0; i < secondary_OAM_index; i++) {
                    sprite_low = 0x00;
                    sprite_high = 0x00;

                    if (sprite_secondary_OAM[i].flags.flip_vertically) {
                        sprite_addr = (ppu_ctrl.ptrn_addr * 0x1000)
                                    + (sprite_secondary_OAM[i].pattern_id * 0x10)
                                    + (7 - (scanlines - sprite_secondary_OAM[i].y));
                    }
                    else {
                        sprite_addr = (ppu_ctrl.ptrn_addr * 0x1000)
                                    + (sprite_secondary_OAM[i].pattern_id * 0x10)
                                    + (scanlines - sprite_secondary_OAM[i].y);
                    }

                    sprite_low = ppuRead(sprite_addr);
                    sprite_high = ppuRead(sprite_addr + 8);

                    if (sprite_secondary_OAM[i].flags.flip_horizontally) {
                        sprite_low = flipByte(sprite_low);
                        sprite_high = flipByte(sprite_high);
                    }

                    sprite_shifter_low[i] = sprite_low;
                    sprite_shifter_high[i] = sprite_high;
                }
            }
        }
    }
    
    if (scanlines == 241 && cycles == 1) {
        ppu_status.v_blank = 1;
        if (ppu_ctrl.generate_nmi) {
            signal_nmi = true;
        }
    }




    uint8_t pixel = 0x00;
    uint8_t palette = 0x00;
    uint8_t sprite_behind_bg = 0x00;

    uint8_t sprite_pixel = 0x00;
    uint8_t sprite_palette = 0x00;

    uint8_t final_pixel = 0x00;
    uint8_t final_palette = 0x00;

    if (ppu_mask.showbg) {
        uint16_t bit_mask = 0x01 << (8 + (7 - fine_x));

        uint8_t pixel_low = (bg_shifter_low & bit_mask) != 0;
        uint8_t pixel_high = (bg_shifter_high & bit_mask) != 0;
        pixel = (pixel_high << 1) | pixel_low;

        uint8_t palette_low = (att_shifter_low & bit_mask) != 0;
        uint8_t palette_high = (att_shifter_high & bit_mask) != 0;
        // curr_palette allows the colors to be changed, but it is not necessary.
        palette = ((palette_high << 1) | palette_low) + curr_palette;
    }

    if (ppu_mask.showsprites) {
        for (int i = 0; i < secondary_OAM_index; i++) {
            if ((cycles - 1) >= sprite_secondary_OAM[i].x && (cycles - 1) < sprite_secondary_OAM[i].x + 8) {
                uint16_t bit_mask = 0x80;

                uint8_t pixel_low = (sprite_shifter_low[i] & bit_mask) != 0;
                uint8_t pixel_high = (sprite_shifter_high[i] & bit_mask) != 0;
                sprite_pixel = (pixel_high << 1) | pixel_low;

                // curr_palette allows the colors to be changed, but it is not necessary.
                sprite_palette = sprite_secondary_OAM[i].flags.palette + 0x04 + curr_palette;
                sprite_behind_bg = sprite_secondary_OAM[i].flags.priority;

                if (sprite_pixel != 0x00) {
                    if (i != 0 && sprite_zero) {
                        sprite_zero = false;
                    }
                    break;
                }
            }
        }
    }

    if (pixel == 0x00  && sprite_pixel > 0x00) {
        final_pixel = sprite_pixel;
        final_palette = sprite_palette;
    }
    else if (pixel > 0x00 && sprite_pixel == 0x00) {
        final_pixel = pixel;
        final_palette = palette;
    }
    else if (pixel > 0x00 && sprite_pixel > 0x00) {
        if (sprite_behind_bg) {
            final_pixel = pixel;
            final_palette = palette;
        }
        else {
            final_pixel = sprite_pixel;
            final_palette = sprite_palette;
        }

        if (sprite_zero) {
            if (ppu_mask.showbg && ppu_mask.showsprites) {
                if (cycles >= 1 && cycles <= 257) {
                    ppu_status.sprite_zerohit = 1;
                }
            }
        }
    }

    // Shows only the visible onscreen pixels.
    if (scanlines >= 0 && scanlines <= 240 && cycles >= 0 && cycles <= 256) {
        drawPixelOnSurface(gui->surface_buff, cycles - 1, scanlines, getColorIndex(final_palette, final_pixel));
    }
    if (show_debug) {
        showPatterntablePixel();
    }
    
    cycles += 1;

    if (cycles >= MAX_COLUMNS)
	{
		cycles = 0;
		
        scanlines += 1;
		if (scanlines >= MAX_SCANLINES)
		{
			scanlines = -1;
			finished = true;

            total_frames += 1;
            odd_frame = !odd_frame;
            SDL_BlitScaled(gui->surface_buff, NULL, gui->surface, &gui->scaled_screen_rect);
            SDL_UpdateWindowSurface(gui->window);
		}
	}
    
    return 0;
}
