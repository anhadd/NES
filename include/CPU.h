/*
Emulated 8 bit 6502 NMOS (1,79 MHz)
NES uses a slightly modified version known as 2A03

Uses Little Endian (Least significant byte first)
Information found on: https://www.nesdev.com/NESDoc.pdf

The 2A03 had a 16-bit address bus, so it supports 64 KB of memory ($0000-$FFFF)
    - Zero Page: $0000-$00FF
    - Memory locations $0000-$07FF are mirrored three times at $0800-$1FFF. 
      For example, any data written to $0000 will also be written to $0800, $1000 and $1800.
    - Memory Mapped IO: $2000-$401F
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

#include "BUS.h"

using namespace std;


#define CARRY_MASK 0x01
#define ZERO_MASK 0x02
#define INTERRUPT_DISABLED_MASK 0x04
#define DECIMAL_MODE_MASK 0x08
#define BREAK_COMMAND_MASK 0x10
#define UNUSED_MASK 0x20
#define OVERFLOW_MASK 0x40
#define NEGATIVE_MASK 0x80

#define ZERO_PAGE_SIZE 0x0100
#define STACK_START ZERO_PAGE_SIZE


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


class CPU {
    public:
        bool execute_nmi;

        void reset();               // Reset interrupt, on startup and when the reset button is pressed.
                                    // jumps to the address located at $FFFC and $FFFD
        void NMI();                 // Non-Maskable Interrupts, cannot be ignored.
                                    // Jumps to the address located at $FFFA and $FFFB

        // Constructor / Destructor
        CPU();
        ~CPU();

        void passBUS(BUS* nesBUS);  // Used for receiving the BUS from the NES.
        bool executeCycle();



        // Public for logging debug information.
        vector<struct instruction> op_lookup;   // Used to lookup data from an opcode.

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

        uint8_t cycles;
        uint32_t total_cycles;      // Counts the total cycles since the start of the program.

        uint8_t cpuRead(uint16_t address);

    private:
        // vector<struct instruction> op_lookup;   // Used to lookup data from an opcode.
        uint8_t opcode;             // Stores the current opcode.

        // // union cpu_memory memory;
        // uint16_t PC;                // Program Counter
        // uint8_t SP;                 // Stack Pointer: Uses offset 0x0100
        //                             // Stack pointer works top-down.
        //                             // No overflow detection, just wraps around form 0x00 to 0xFF.
        // union status_register status;
        //                             // Has flags for various things.

        // uint8_t accumulator;        // Stores results of arithmetic and logic operations.
        //                             // Can also be set to value form memory.
        // uint8_t X;
        // uint8_t Y;
        uint16_t temp;              // Stores the temporary results of instructions.
        uint8_t read_data;          // Stores the data read form a read, so that it can be reused.

        // Used to store the final address, after considering addressing modes.
        uint16_t absolute_address;
        // Used to store the offset of branches, if applicable.
        int8_t relative_offset;
        // Used to store the amount of cycles that an instruction took.
        // uint8_t cycles;
        bool additional_cycle;      // Used to store whether an instruction might take an extra cycle.
        // uint32_t total_cycles;      // Counts the total cycles since the start of the program.
        
        enum addressing_mode mode;  // Used to store the current addressing mode.

        BUS* bus;                   // The BUS that takes care of memory reads and writes.
        uint16_t rom_address;       // Address to start of the rom.

        // Interrupt priority: reset > NMI > IRQ
        // The NES takes 7 CPU cycles to begin executing the interrupt handler.
        void IRQ();                 // Maskable interrupts, ignored if interrupt_disabled is set.
                                    // jumps to the address located at $FFFE and $FFFF
        void decrementSP();         // Decrements the stack pointer (SP);
        void incrementSP();         // Increments the stack pointer (SP);
        
        // enum addressing_mode mode;

        // Operations: Can be 1, 2, or 3 bytes long.
        // First byte is always the opcode, the rest are arguments.
        void ADC(); void AND(); void ASL(); void BCC(); void BCS(); void BEQ(); void BIT(); void BMI();
        void BNE(); void BPL(); void BRK(); void BVC(); void BVS(); void CLC(); void CLD(); void CLI();
        void CLV(); void CMP(); void CPX(); void CPY(); void DEC(); void DEX(); void DEY(); void EOR();
        void INC(); void INX(); void INY(); void JMP(); void JSR(); void LDA(); void LDX(); void LDY();
        void LSR(); void NOP(); void ORA(); void PHA(); void PHP(); void PLA(); void PLP(); void ROL();
        void ROR(); void RTI(); void RTS(); void SBC(); void SEC(); void SED(); void SEI(); void STA();
        void STX(); void STY(); void TAX(); void TAY(); void TSX(); void TXA(); void TXS(); void TYA();
        void UNK(); // For unknown opcodes.

        // Illegal opcode operations.
        void SRE(); void ISC(); void LAX(); void SAX(); void DCP(); void SLO(); void RLA(); void RRA();
        void ANC(); void ALR(); void ARR(); void AXS(); void LAS(); void AHX(); void SHY(); void SHX();
        void TAS();

        // Helper functions.
        void checkBranch(bool flag);
        void pushPCToStack();
        void pushStatusToStack(bool is_instruction);

        bool readAddress();

        // uint8_t cpuRead(uint16_t address);
        uint8_t cpuWrite(uint16_t address, uint8_t value);
};

// The structure of an instruction with all the data that is required.
struct instruction {
    uint8_t opcode;
    char opname[4];
    void (CPU::*opFunction)(void);
    enum addressing_mode opmode;
    uint8_t opcycles;
    bool extra_cycle;
};

#endif
