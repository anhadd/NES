/*
Emulated 8 bit 6502 NMOS (1,79 MHz)
NES uses a slightly modified version known as 2A03

Uses Little Endian (Least significant byte first)
Information found on: https://www.nesdev.com/NESDoc.pdf

The 2A03 had a 16-bit address bus, so it supports 64 KB of memory ($0000-$FFFF)
    - Zero Page: $0000-$00FF
    - Memory locations $0000-$07FF are mirrored three times at $0800-$1FFF. 
      For example, any data written to $0000 will also be written to $0800, $1000 and $1800.
      Memory Mapped IO: $2000-$401F
    - Locations $2000-$2007 are mirrored every 8 bytes in the region $2008-$3FFF 
      The remaining registers follow this mirroring. 
    - SRAM (WRAM) is the Save RAM, the addresses used to access RAM in the cartridges for storing save
      games.
    - From $8000 onwards is the addresses allocated to cartridge PRG-ROM.
      Games with only one 16 KB bank of PRG-ROM will load it into both $8000 and $C000.
      Games with two 16 KB PRG-ROM banks will load one into $8000 and the other into $C000.
      Games with more than two banks use memory mappers to determine which banks to load into memory.

There are 56 different instructions, and including the variations possible there are
a total of 151 valid opcodes out of the possible 256.
*/

#ifndef CPU_H
#define CPU_H

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "ROM.h"

using namespace std;

// A register with status bits which can be set.
union status_register {
    struct {
        uint8_t carry : 1;                  // C
        uint8_t zero : 1;                   // Z
        uint8_t interrupt_disabled : 1;     // I
        uint8_t decimal_mode : 1;           // D: Not implemented in NES
        uint8_t break_command : 1;          // B
        uint8_t unused : 1;                 // Unused
        uint8_t overflow : 1;               // V
        uint8_t negative : 1;               // N
    };
    uint8_t full;
};

// Possible addressing modes.
enum addressing_mode {
    ZPN,                // Zero Page (Normal)
    ZPX,                // Zero Page X
    ZPY,                // Zero Page Y
    ABS,                // Absolute
    ABX,                // Absolute X
    ABY,                // Absolute Y
    IND,                // Indirect
    IZX,                // Indirect X
    IZY,                // Indirect Y (Indirect Indexed)
    IMP,                // Implied
    ACC,                // Accumulator
    IMM,                // Immediate
    REL                 // Relative
};

struct instruction;

// DONE: CHECK HOW TO DO ADDRESSING MODES -> PROBABLY READ INTO AN FINAL ADDRESS AND USE THAT IN THE FUNCTIONS.
class CPU {
    public:
        vector<struct instruction> op_lookup;   // Used to lookup data from an opcode.
        uint8_t opcode;             // Stores the current opcode.

        // union cpu_memory memory;
        uint8_t memory[0x10000];
        uint16_t PC;                // Program Counter
        uint8_t SP;                 // Stack Pointer: Uses offset 0x0100
                                    // Stack pointer works top-down.
                                    // No overflow detection, just wraps around form 0x00 to 0xFF.
        union status_register status;
                                    // Has flags for various things.

        uint8_t accumulator;        // Stores results of arithmetic and logic operations.
                                    // Can also be set to value form memory.
        uint8_t X;
        uint8_t Y;
        uint16_t temp;              // Stores the temporary results of instructions.

        // Used to store the final address, after considering addressing modes.
        uint16_t absolute_address;
        // Used to store the offset of branches, if applicable.
        int8_t relative_offset;
        // Used to store the amount of cycles that an instruction took.
        uint8_t cycles;
        uint32_t total_cycles;      // Counts the total cycles since the start of the program.
        
        enum addressing_mode mode;  // Used to store the current addressing mode.

        ROM rom;                    // Contains all rom header data.
        uint16_t rom_address;       // Address to start of the rom.

        // Interrupt priority: reset > NMI > IRQ
        // The NES takes 7 CPU cycles to begin executing the interrupt handler.
        void IRQ();                 // Maskable interrupts, ignored if interrupt_disabled is set.
                                    // jumps to the address located at $FFFE and $FFFF
        void NMI();                 // Non-Maskable Interrupts, cannot be ignored.
                                    // Jumps to the address located at $FFFA and $FFFB
        void reset();               // Reset interrupt, on startup and when the reset button is pressed.
                                    // jumps to the address located at $FFFC and $FFFD
        void decrementSP();         // Decrements the stack pointer (SP);
        
        // enum addressing_mode mode;

        // Operations: Can be 1, 2, or 3 bytes long.
        // First byte is always the opcode, the rest are arguments.
        bool ADC(); bool AND(); bool ASL(); bool BCC(); bool BCS(); bool BEQ(); bool BIT(); bool BMI();
        bool BNE(); bool BPL(); bool BRK(); bool BVC(); bool BVS(); bool CLC(); bool CLD(); bool CLI();
        bool CLV(); bool CMP(); bool CPX(); bool CPY(); bool DEC(); bool DEX(); bool DEY(); bool EOR();
        bool INC(); bool INX(); bool INY(); bool JMP(); bool JSR(); bool LDA(); bool LDX(); bool LDY();
        bool LSR(); bool NOP(); bool ORA(); bool PHA(); bool PHP(); bool PLA(); bool PLP(); bool ROL();
        bool ROR(); bool RTI(); bool RTS(); bool SBC(); bool SEC(); bool SED(); bool SEI(); bool STA();
        bool STX(); bool STY(); bool TAX(); bool TAY(); bool TSX(); bool TXA(); bool TXS(); bool TYA();
        bool UNK(); // For unknown opcodes.

        // Helper function for branches.
        bool checkBranch(bool flag);

        bool readAddress();
        bool executeCycle();
        bool loadRom(char* romName);

        // Constructor / Decstructor
        CPU();
        ~CPU();
};

// The structure of an instruction with all the data that is required.
struct instruction {
    uint8_t opcode;
    bool (CPU::*opFunction)(void);
    enum addressing_mode opmode;
    uint8_t opcycles;
};

#endif
