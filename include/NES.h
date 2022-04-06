// CHIP-8 uses a 64x32 monochrome display:
// (0,0)	        (63,0)
//
// (0,31)	        (63,31)

// Uses sprites, which are at most 15 bytes (8x15).
// There are 36 instructions, they are all 16-bit.

#ifndef NES_H
#define NES_H

#include <iostream>
#include <fstream>

#include <thread>
#include <vector>

using namespace std;


const unsigned char fontset[16*5] = {
	0xF0, 0x90, 0x90, 0x90, 0xF0,		// 0
	0x20, 0x60, 0x20, 0x20, 0x70,		// 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0,		// 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0,		// 3
	0x90, 0x90, 0xF0, 0x10, 0x10,		// 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0,		// 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0,		// 6
	0xF0, 0x10, 0x20, 0x40, 0x40,		// 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0,		// 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0,		// 9
	0xF0, 0x90, 0xF0, 0x90, 0x90,		// A
	0xE0, 0x90, 0xE0, 0x90, 0xE0,		// B
	0xF0, 0x80, 0x80, 0x80, 0xF0,		// C
	0xE0, 0x90, 0x90, 0x90, 0xE0,		// D
	0xF0, 0x80, 0xF0, 0x80, 0xF0,		// E
	0xF0, 0x80, 0xF0, 0x80, 0x80		// F
};


class NES {
	public:
		bool drawFlag;

		bool display[64][32];			// The display of CHIP-8 (64x32).
		bool keys[16];					// Used for the 16 keys that CHIP-8 has (1-9 A-F).

		NES();
		void executeCycle(bool &play_sound);
		int loadRom(const char* romName);

	private:
		uint16_t curr_op;				// Stores the current operation.
		uint8_t x;
		uint8_t y;
		uint8_t h;
		bool toggled;
		bool incrementPC;

		uint8_t memory[4096];     		// 4096 bytes of memory.
		uint16_t stack[16];        		// 16 16-bit registers used to store return pointers.
		
		uint8_t V[16];                	// 16 8-bit registers. 
										// VF (V[16]) should no be used by the program since it is used as a flag.

		uint16_t I;             		// 16-bit register used to store memory addresses.
		uint8_t delay_timer;    		// 8-bit register. Decremented at 60 Hz.
		uint8_t sound_timer;    		// 8-bit register. Decremented at 60 Hz.

		uint16_t PC;            		// 16-bit program counter.
		uint8_t SP;             		// 16-bit stack pointer.

		uint16_t font_addr;				// Address of the start of the fonts.
		uint16_t rom_addr;				// Address of the start of the rom.
};

#endif
