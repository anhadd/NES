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

union cpu_memory {
    struct {
        char zero_page[0x0100 - 0x0000];
        char stack[0x0200 - 0x0100];
        char ram[0x0800 - 0x0200];
        char ram_mirrors[0x2000 - 0x0800];
        char io1[0x2008 - 0x2000];
        char io1_mirrors[0x4000 - 0x2008];
        char io2[0x4020 - 0x4000];
        char expansion_rom[0x6000 - 0x4020];
        char sram[0x8000 - 0x6000];
        char prg_lower[0xC000 - 0x8000];
        char prg_upper[0x10000 - 0xC000];
    };
    char full[0x10000];
};

union status_register {
    struct {
        uint8_t negative : 1;               // N
        uint8_t overflow : 1;               // V
        uint8_t unused : 1;                 // Unused
        uint8_t break_command : 1;          // B
        uint8_t decimal_mode : 1;           // D: Not implemented in NES
        uint8_t interrupt_disabled : 1;     // I
        uint8_t zero : 1;                   // Z
        uint8_t carry : 1;                  // C
    };
    uint8_t full;
};

enum addressing_mode {
    ZeroPage,
    ZeroPage_X,
    ZeroPage_Y,
    Absolute,
    Absolute_X,
    Absolute_Y,
    Indirect,
    Indirect_X,
    Indirect_Y,
    Implied,
    Accumulator,
    Immediate,
    Relative,
    IndirectIndexed
};

// TODO: CHECK HOW TO DO ADDRESSING MODES -> PROBABLY READ INTO AN FINAL ADDRESS AND USE THAT IN THE FUNCTIONS.
class CPU {
    public:
        union cpu_memory memory;
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

        // Interrupt priority: reset > NMI > IRQ
        // The NES takes 7 CPU cycles to begin executing the interrupt handler.
        void IRQ();                      // Maskable interrupts, ignored if interrupt_disabled is set.
                                    // jumps to the address located at $FFFE and $FFFF
        void NMI();                      // Non-Maskable Interrupts, cannot be ignored.
                                    // Jumps to the address located at $FFFA and $FFFB
        void reset();                    // Reset interrupt, on startup and when the reset button is pressed.
                                    // jumps to the address located at $FFFC and $FFFD
        
        enum addressing_mode mode;

        // Operations: Can be 1, 2, or 3 bytes long.
        // First byte is always the opcode, the rest are arguments.
        bool ADC(); bool AND(); bool ASL(); bool BCC(); bool BCD(); bool BEQ(); bool BIT(); bool BMI();
        bool BNE(); bool BPL(); bool BRK(); bool BVC(); bool BVS(); bool CLC(); bool CLD(); bool CLI();
        bool CLV(); bool CMP(); bool CPX(); bool CPY(); bool DEC(); bool DEX(); bool DEY(); bool EOR();
        bool INC(); bool INX(); bool INY(); bool JMP(); bool JSR(); bool LDA(); bool LDX(); bool LDY();
        bool LSR(); bool NOP(); bool ORA(); bool PHA(); bool PHP(); bool PLA(); bool PLP(); bool ROL();
        bool ROR(); bool RTI(); bool RTS(); bool SBC(); bool SEC(); bool SED(); bool SEI(); bool STA();
        bool STX(); bool STY(); bool TAX(); bool TAY(); bool TSX(); bool TXA(); bool TXS(); bool TYA();
        bool INV(); // For invalid opcodes.

        // Constructor / Decstructor
        CPU();
        ~CPU();
};

#endif
