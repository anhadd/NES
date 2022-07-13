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
#include "ROM.h"

using namespace std;

#define MAX_COLUMNS         341
#define MAX_SCANLINES       261
#define TILE_SIZE_IN_BYTES  0x10
#define PATTERNTABLE_SIZE   0x1000

#define NAMETABLE_START     0x2000
#define NAMETABLE_SIZE      0x0400
#define NAMETABLE_RANGE     0x03FF
#define ATTRIBUTE_START     0x23C0

#define PALETTE_START       0x3F00
#define PALETTE_RANGE       0x3F

#define GRAYSCALE_MASK      0x30

#define PPU_REGISTERS_START 0x2000
#define PPU_REGISTER_RANGE  0x0007
#define CONTROL     0x2000
#define MASK        0x2001
#define STATUS      0x2002
#define OAM_ADDR    0x2003
#define OAM_DATA    0x2004
#define SCROLL      0x2005
#define PPU_ADDR    0x2006
#define PPU_DATA    0x2007

#define FRAMES_PER_DEBUG_UPDATE 60


// Struct for the colors of the NES palette.
struct Color {
    uint16_t index;
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

// PPU Control register.
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

// PPU Mask register.
union PPUMASK {
    struct {
        uint8_t grayscale : 1;
        uint8_t showbg_left : 1;
        uint8_t showsprites_left : 1;
        uint8_t showbg : 1;
        uint8_t showsprites : 1;
        uint8_t emphasis_r : 1;
        uint8_t emphasis_g : 1;
        uint8_t emphasis_b : 1;
    };
    uint8_t full;
};

// PPU Status register.
union PPUSTATUS {
    struct {
        uint8_t prev_lsb : 5;
        uint8_t sprite_overflow : 1;
        uint8_t sprite_zerohit : 1;
        uint8_t v_blank : 1;
    };
    uint8_t full;
};


// Loopy register, convenient way to store scrolling information made by Loopy.
// Is also used as the PPU Address register.
// The repeating screen is fixed here! Apparently there is a big difference between:
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

// Struct for storing sprite information in OAM.
// Used for accessing the sprite information in OAM memory.
struct OAM_sprite {
    uint8_t y;
    uint8_t pattern_id;
    union sprite_attributes flags;
    uint8_t x;
};



// NES PPU.
class PPU {
    public:
        bool signal_nmi;        // Set when the CPU should execute an NMI.
        bool frame_finished;    // Set when the PPU finished an entire frame.
        bool show_debug;        // Set if debug windows should be updated.

        uint8_t curr_palette;   // Stores an index for the current palette, allows for changing colors in game.
                                // This is completely unnecessary and not part of the real NES, but it is nice to have.

        // ===== These variables are set to public in order to log debug information in the NES file.
        union loopy_register ppu_addr;  // Loopy register containing scroll information, also used as the PPU address register.
        uint8_t fine_x;                 // Stores the pixel x offset inside a tile.

        uint16_t cycles;                // Stores the current PPU cycle, corresponds to the x position on the screen.
        int16_t scanlines;              // Stores the current scanline, corresponds to the y position on the screen.
        // =====

        // Constructor / Destructor.
        PPU();
        ~PPU();

        void passGUI(GUI* nesGUI);  // Used for receiving the GUI from the NES.
        void passROM(ROM* nesROM);  // Used for receiving the ROM from the NES.
        bool executeCycle();        // Executes a single PPU cycle.
        void reset();               // Resets the PPU.

        uint8_t ppuRead(uint16_t address);                  // Read from PPU memory.
        uint8_t ppuWrite(uint16_t address, uint8_t value);  // Write to PPU memory.

        uint8_t readRegister(uint16_t address);                 // Read from PPU registers.
        uint8_t writeRegister(uint16_t address, uint8_t value); // Write to PPU registers.

    private:
        GUI* gui;
        ROM* rom;
        
        uint16_t total_frames;                  // Used for updating the debug screens every number of frames.
        bool address_latch;                     // The address latch used for PPU writes. Determines the effects of a write to the address and scroll registers.
        bool odd_frame;                         // Set when the PPU is executing an odd frame. Used for skipped a cycle every odd frame.

        struct Color curr_color;                // Used for temporarily storing the current color that needs to be displayed on the screen.
        vector<struct Color> palette_lookup;    // Lookup table containing all the colors of the NES palette.

        uint8_t ppu_nametable[2][0x0400];       // PPU memory: 2x 1KB nametables.
        uint8_t ppu_palette[0x0020];            // PPU memory: Palettes.
        
        PPUCTRL ppu_ctrl;                       // PPU Control register.
        PPUMASK ppu_mask;                       // PPU Mask register.
        PPUSTATUS ppu_status;                   // PPU Status register.
        uint8_t oam_addr;                       // PPU OAM address register.
        uint8_t oam_data;                       // PPU OAM data register.

        union loopy_register ppu_buff;          // Buffer for ppu_addr. Writes to ppu_addr go here first before being transferred to ppu_addr.

        uint8_t ppu_data;                       // PPU Data register.
        uint8_t bus_value;                      // Current value in the BUS. Used for emulating open BUS behavior.

        // There is only a single OAM and a single secondary OAM. 
        // The ones of type uint8_t is used for indexing by byte, which is useful for read and writes.
        // The ones of type OAM_sprite are indexed by sprite, and are useful for easily getting information about sprites when rendering.
        uint8_t* OAM;                           // Stores sprite information. Indexed by byte.
        OAM_sprite sprite_OAM[0x40];            // Stores sprite information. Indexed by sprite.
        
        uint8_t* secondary_OAM;                 // Stores information of the sprites on the next scanline. Indexed by byte.
        OAM_sprite sprite_secondary_OAM[0x08];  // Stores information of the sprites on the next scanline. Indexed by Sprite.
        uint8_t secondary_OAM_index;            // Stores the amount of sprites stored in secondary OAM. Used for emulating sprite overflows.

        // The high and low bytes of sprites are used for getting the correct palette color for every sprite.
        uint8_t sprite_shifter_high[0x08];      // Stores the high byte of every sprite in secondary_OAM.
        uint8_t sprite_shifter_low[0x08];       // Stores the low byte of every sprite in secondary_OAM.

        bool sprite_zero_in_sOAM;               // Set if sprite 0 (OAM[0]) is present in the secondary OAM.
        bool sprite_zero_rendering;             // Set if sprite 0 (OAM[0]) is currently being rendered.

        uint8_t data_read_buffer;               // For storing the previously read value from PPU memory. Used for emulating delayed reads.

        // Background rendering variables
        uint16_t bg_shifter_high;               // Shifter register used to store the high byte of the background tiles.
        uint16_t bg_shifter_low;                // Shifter register used to store the low byte of the background tiles.
        uint16_t att_shifter_high;              // Stores the high byte of the background attribute corresponding to the background tile.
        uint16_t att_shifter_low;               // Stores the low byte of the background attribute corresponding to the background tile.

        uint8_t bg_nametable;                   // Stores the next tile index in the nametable that needs to be rendered.
        uint8_t bg_attribute;                   // Stores the next attribute data of the background tile that needs to be rendered.
        uint8_t bg_low;                         // Stores the next low byte of the background tile that needs to be rendered.
        uint8_t bg_high;                        // Stores the next high byte of the background tile that needs to be rendered.

        uint8_t bg_pixel;                       // Stores the index of the color in the palette for the bg.
        uint8_t bg_palette;                     // Stores the index of the palette for the bg.
        bool sprite_behind_bg;                  // Whether the sprite is behind the background or not.

        uint8_t sprite_pixel;                   // Stores the index of the color in the palette for the sprite.
        uint8_t sprite_palette;                 // Stores the index of the palette for the sprite.

        uint8_t final_pixel;                    // Stores the index of the color in the palette for the final rendered pixel.
        uint8_t final_palette;                  // Stores the index of the palette for the final rendered pixel.

        uint16_t getColorIndex(uint8_t palette, uint8_t index);     // Used for getting the absolute index of a color with a certain palette and index inside that palette.
        void drawPixel(vector<uint8_t> &pixels, uint16_t x, uint16_t y, uint16_t color_index);          // Draw a single pixel.
        void drawPixelOnSurface(SDL_Surface *surface, uint16_t x, uint16_t y, uint16_t color_index);    // Draw a single pixel on a surface.
        void drawDebugPixels();                                     // Update the debug screens.

        void loadShifters();                    // Loads the next byte of data into the shifter registers.
        void updateShifters();                  // Updates the shifters, which means shifting the data by 1 bit.
        void incrementPPUAddr();                // Increments the PPU address depending on the increment mode.
        uint8_t flipByte(uint8_t byte);

        void incrementCoarseX();                // Move to the next horizontal tile.
        void incrementCoarseY();                // Move to the next vertical tile.
        void incrementFineY();                  // Move to the next vertical pixel.

        void loadAttributeByte();               // Load the attribute byte.
        void loadTileByte(bool high_byte);      // Load the low or high tile byte.
};



#endif
