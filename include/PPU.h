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
        uint8_t nametable_x : 1;
        uint8_t nametable_y : 1;
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


// The repeating screen is fixed here! Apparently there is a real difference between:
    // uint16_t X : 5
    // uint8_t X : 5
union loopy_register {
    struct {
        uint16_t coarse_x : 5;
        uint16_t coarse_y : 5;
        uint16_t nametable_x : 1;
        uint16_t nametable_y : 1;
        uint16_t fine_y : 3;
        uint16_t padding : 1;
    };
    uint16_t full;
};

// Sprite attributes:

// 76543210
// ||||||||
// ||||||++- Palette (4 to 7) of sprite
// |||+++--- Unimplemented (read 0)
// ||+------ Priority (0: in front of background; 1: behind background)
// |+------- Flip sprite horizontally
// +-------- Flip sprite vertically
union sprite_attributes {
    struct {
        uint8_t palette : 2;
        uint8_t unused : 3;
        uint8_t priority : 1;
        uint8_t flip_horizontally : 1;
        uint8_t flip_vertically : 1;
    };
    uint8_t full;
};


// Used for accessing the sprite information in OAM memory.
struct OAM_sprite {
    uint8_t y;
    uint8_t pattern_id;
    union sprite_attributes flags;
    uint8_t x;
};




class PPU {
    public:
        bool vertical_mirorring;                // 0: Horizontal / 1: Vertical
        bool signal_nmi;
        bool finished;
        bool show_debug;

        uint8_t ppu_patterntable[0x2000];       // PPU memory
        uint8_t* OAM;

        uint8_t curr_palette;

        PPU();
        ~PPU();

        void passGUI(GUI* nesGUI);
        bool executeCycle();
        void reset();

        uint8_t ppuRead(uint16_t address);
        uint8_t ppuWrite(uint16_t address, uint8_t value);

        uint8_t readRegister(uint16_t address);
        uint8_t writeRegister(uint16_t address, uint8_t value);

    private:
        GUI* gui;
        
        uint16_t total_frames;                  // Used for updating the debug screens.
        uint16_t cycles;
        int16_t scanlines;
        bool address_latch;
        bool odd_frame;

        struct color curr_color;
        vector<struct color> palette_lookup;

        uint8_t ppu_nametable[2][0x0400];       // PPU memory: 2x 1KB nametables
        uint8_t ppu_palette[0x0020];            // PPU memory
        
        PPUCTRL ppu_ctrl;
        PPUMASK ppu_mask;
        PPUSTATUS ppu_status;
        uint8_t oam_addr;
        uint8_t oam_data;
        uint8_t ppu_scroll;

        // uint16_t ppu_addr;
        union loopy_register ppu_addr;
        union loopy_register ppu_buff;
        uint8_t fine_x;

        uint8_t ppu_data;

        OAM_sprite sprite_OAM[0x40];
        OAM_sprite sprite_secondary_OAM[0x08];
        uint8_t* secondary_OAM;
        uint8_t secondary_OAM_index;

        uint8_t sprite_shifter_high[0x08];
        uint8_t sprite_shifter_low[0x08];

        bool sprite_zero;

        uint8_t data_read_buffer;

        // Background rendering variables
        uint16_t bg_shifter_high;
        uint16_t bg_shifter_low;
        uint16_t att_shifter_high;
        uint16_t att_shifter_low;

        uint8_t bg_nametable;
        uint8_t bg_attribute;
        uint8_t bg_low;
        uint8_t bg_high;

        uint16_t getColorIndex(uint8_t palette, uint8_t index);
        void drawPixelOnSurface(SDL_Surface *surface, uint16_t x, uint16_t y, uint16_t color_index);
        void showPatterntablePixel();

        void loadShifters();
        void updateShifters();
        void incrementPPUAddr();
        uint8_t flipByte(uint8_t byte);
};



#endif
