#include "../include/PPU.h"



PPU::PPU() {
    // Constructor
    cycles = 0;
    scanlines = 0;
    finished = false;
    // TODO: SET TO FALSE LATER !!!
    v_blank = true;

    fill(begin(OAM), end(OAM), 0);
    fill(begin(secondary_OAM), end(secondary_OAM), 0);

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

void PPU::passBUS(BUS* nesBUS) {
    bus = nesBUS;
}

void PPU::passGUI(GUI* nesGUI) {
    gui = nesGUI;
}

uint8_t PPU::ppuRead(uint16_t address) {
    return bus->busReadPPU(address);
}

uint8_t PPU::ppuWrite(uint16_t address, uint8_t value) {
    bus->busWritePPU(address, value);
    return 0;
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
        drawPixel(cycles, scanlines, pixel);
    }
}

bool PPU::executeCycle() {
    // Execute a single cycle.
    // TODO: IMPLEMENT THIS !!!
    if (scanlines == 241 && cycles == 1) {
        v_blank = true;
    }
    else if (scanlines == -1 && cycles == 1) {
        v_blank = false;
    }


    // TODO: Use the correct color, not just a random one.
    // drawPixel(cycles - 1, scanlines, rand() % 0x3F);
    showPatterntablePixel();

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
