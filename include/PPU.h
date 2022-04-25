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

#include "BUS.h"

using namespace std;


class PPU {
    public:
        BUS* bus;
        

        PPU();
        ~PPU();

        void passBUS(BUS* nesBUS);

        uint8_t ppuRead(uint16_t address);
        uint8_t ppuWrite(uint16_t address, uint8_t value);

        bool executeCycle();
};



#endif
