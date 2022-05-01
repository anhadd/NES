/*
From: https://www.eecg.utoronto.ca/~pc/courses/432/2008/projects/ppu.pdf

VRAM (IP) – The VRAM stores video data in the following format for the PPU to process:
    (1) Pattern Table – Contains 256 tiles used for Background and Sprites. Each tile consists of
        an 8x8 pixel bitmap with 2bit depth (4 colors).
    (2) Name Table – Each frame contains 32x30 tiles (256x240 pixels) with 8bit tile name.
    (3) Attribute Table – 2bit color information. Combine with the 2bit in Pattern table to create a
        total of 16 different colors. Each frame contains 16x15 attribute blocks while each attribute
        block defines the color of a 16x16 pixel bitmap.
    (4) Palette memory – Color map; it contains separate entries for background and sprite.

VRAM offset:
    0000h-0FFFh: Pattern Table 0
    1000h-1FFFh: Pattern Table 1
    2000h-23FFh: Name Table 0 and Attribute Table 0 (32x30 BG Map)
    2400h-27FFh: Name Table 1 and Attribute Table 1 (32x30 BG Map)
    2800h-2BFFh: Name Table 2 and Attribute Table 2 (32x30 BG Map)
    2C00h-2FFFh: Name Table 3 and Attribute Table 3 (32x30 BG Map)
    3F00h-3F1Fh: Background and Sprite Palettes


SRAM(IP) – A block ram for storing sprite attribute data. It is capable of displaying up to 64 sprite
patterns on screen at a time. The sprite offset defines the sprite’s vertical/horizontal positions, the tile 9
name defined in the pattern table and background priority of the sprite. The sprite attribute together with
the Pattern Table will be used to render the sprites. For more information see [1][2][3].

SRAM offset:
    00h-FFh - Sprite Attributes (256 bytes, for 64 sprites / 4 bytes per sprites)
    Byte 0 – Vertical Position (0 to 239)
    Byte 1 – Tile Number (0 to 256, sprite share the same pattern table as the background)
    Byte 2 – bit 5 Background Priority (0-Sprite in front of background, 1-sprite behind BG, sprite disabled)
    bit 1-0 Sprite palette
    Byte 3 – Horizontal Position (0 to 255)
*/





#ifndef PPU_H
#define PPU_H

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include "GUI.h"

using namespace std;

#define MAX_COLUMNS 341
#define MAX_SCANLINES 261

#define CONTROL     0x2000
#define MASK        0x2001
#define STATUS      0x2002
#define OAM_ADDR    0x2003
#define OAM_DATA    0x2004
#define SCROLL      0x2005
#define PPU_ADDR    0x2006
#define PPU_DATA    0x2007


struct color {
    uint16_t index;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};
union PPUCTRL {
    struct {
        uint8_t nametable_addr : 2;
        uint8_t ppu_increment : 1;
        uint8_t ptrn_addr : 1;
        uint8_t bgr_addr : 1;
        uint8_t sprite_size : 1;
        uint8_t master_slave : 1;
        uint8_t generate_nmi : 1;
    };
    uint8_t full;
};

union PPUMASK {
    struct {
        uint8_t greyscale : 1;
        uint8_t showbg_left : 1;
        uint8_t showsprites_left : 1;
        uint8_t showbg : 1;
        uint8_t showsprites : 1;
        uint8_t emph_r : 1;
        uint8_t emph_g : 1;
        uint8_t emph_b : 1;
    };
    uint8_t full;
};

union PPUSTATUS {
    struct {
        uint8_t prev_lsb : 5;
        uint8_t sprite_overflow : 1;
        uint8_t sprite_zerohit : 1;
        uint8_t v_blank : 1;
    };
    uint8_t full;
};






class PPU {
    public:
        GUI* gui;
        
        uint16_t cycles;
        int16_t scanlines;
        bool finished;
        bool address_latch;

        struct color curr_color;
        vector<struct color> palette_lookup;

        uint8_t ppu_patterntable[0x2000];       // PPU memory
        uint8_t ppu_nametable[0x1000];          // PPU memory
        uint8_t ppu_palette[0x0020];            // PPU memory
        
        PPUCTRL ppu_ctrl;
        PPUMASK ppu_mask;
        PPUSTATUS ppu_status;
        uint8_t oam_addr;
        uint8_t oam_data;
        uint8_t ppu_scroll;
        uint16_t ppu_addr;
        uint8_t ppu_data;

        uint8_t OAM[0xFF];
        uint8_t secondary_OAM[0x20];

        uint8_t data_read_buffer;
        uint8_t curr_palette;

        PPU();
        ~PPU();

        void reset();
        void passGUI(GUI* nesGUI);

        uint8_t ppuRead(uint16_t address);
        uint8_t ppuWrite(uint16_t address, uint8_t value);

        uint8_t readRegister(uint16_t address);
        uint8_t writeRegister(uint16_t address, uint8_t value);

        uint16_t getColorIndex(uint8_t palette, uint8_t index);
        void drawPixel(uint16_t x, uint16_t y, uint16_t color_index);
        void showPatterntablePixel();
        void incrementPPUAddr();

        bool executeCycle();
};



#endif
