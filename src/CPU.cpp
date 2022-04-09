#include "../include/CPU.h"



// bool ADC(); bool AND(); bool ASL(); bool BCC(); bool BCD(); bool BEQ(); bool BIT(); bool BMI();
// bool BNE(); bool BPL(); bool BRK(); bool BVC(); bool BVS(); bool CLC(); bool CLD(); bool CLI();
// bool CLV(); bool CMP(); bool CPX(); bool CPY(); bool DEC(); bool DEX(); bool DEY(); bool EOR();
// bool INC(); bool INX(); bool INY(); bool JMP(); bool JSR(); bool LDA(); bool LDX(); bool LDY();
// bool LSR(); bool NOP(); bool ORA(); bool PHA(); bool PHP(); bool PLA(); bool PLP(); bool ROL();
// bool ROR(); bool RTI(); bool RTS(); bool SBC(); bool SEC(); bool SED(); bool SEI(); bool STA();
// bool STX(); bool STY(); bool TAX(); bool TAY(); bool TSX(); bool TXA(); bool TXS(); bool TYA();

CPU::CPU() {
    fill(begin(memory.full), end(memory.full), 0);

    PC = 0x0000;
    SP = 0x00;

    accumulator = 0;

    X = 0;
    Y = 0;

    absolute_address = 0x0000;
    // TODO: CONTINUE FIXING THIS LOOKUP TABLE STUFF
    op_lookup = {
        // opcode, opmode, opcycles, opFunction 
        {0xFF, Relative, 7, &ADC()}
    };
}

bool CPU::ADC() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}


bool CPU::AND() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.

    return 0;
}


bool CPU::executeCycle() {
    // Execute a single cycle.
    if (cycles == 0) {
        // If opcode -> set mode -> set cycles -> call getaddress -> call opcode function.
    }
    cycles -= 1;
    return 0;
}


bool CPU::readAddress() {
    // Reads an address from the PC into absolute_address depending on the mode.
    uint8_t byte1 = 0x00;
    uint8_t byte2 = 0x00;
    uint16_t indirect_address = 0x0000;

    switch (mode) {
        case ZeroPage:
            // The absolute address is just 1 byte.
            byte1 = memory.full[PC];
            absolute_address = byte1;
            PC += 1;
            break;
        case ZeroPage_X:
            // The absolute address is 1 byte plus X.
            byte1 = memory.full[PC];
            absolute_address = (byte1 + X) % 0x0100;
            PC += 1;
            break;
        case ZeroPage_Y:
            // The absolute address is 1 byte plus Y.
            byte1 = memory.full[PC];
            absolute_address = (byte1 + Y) % 0x0100;
            PC += 1;
            break;
        
        case Absolute:
            // The absolute address is 2 bytes swapped around.
            byte1 = memory.full[PC];
            byte2 = memory.full[PC + 1];
            absolute_address = (byte2 << 8) | (byte1);
            PC += 2;
            break;
        case Absolute_X:
            // The absolute address is 2 bytes swapped around plus X.
            byte1 = memory.full[PC];
            byte2 = memory.full[PC + 1];
            absolute_address = ((byte2 << 8) | (byte1)) + X;
            // Add extra cycle since it crosses a page boundary.
            if ((absolute_address & 0xFF00) != (byte2 << 8)) {
                cycles += 1;
            }
            PC += 2;
            break;
        case Absolute_Y:
            // The absolute address is 2 bytes swapped around plus Y.
            byte1 = memory.full[PC];
            byte2 = memory.full[PC + 1];
            absolute_address = ((byte2 << 8) | (byte1)) + Y;
            // Add extra cycle since it crosses a page boundary.
            if ((absolute_address & 0xFF00) != (byte2 << 8)) {
                cycles += 1;
            }
            PC += 2;
            break;
        
        case Indirect:
            // The absolute address is at the location of the 2 bytes swapped around.
            byte1 = memory.full[PC];
            byte2 = memory.full[PC + 1];
            indirect_address = (byte2 << 8) | (byte1);
            // A bug in the NES causes the +1 to not move to the next page when the lower byte is 0x00FF.
            if (byte1 == 0x00FF) {
                byte1 = memory.full[indirect_address];
                byte2 = memory.full[indirect_address & 0xFF00];
            }
            else {
                byte1 = memory.full[indirect_address];
                byte2 = memory.full[indirect_address + 1];
            }
            absolute_address = ((byte2 << 8) | (byte1)) + Y;
            PC += 2;
            break;
        case Indirect_X:
            // Take 1 byte + X, and take the address from there (with wraparound).
            indirect_address = memory.full[PC] + X;
            byte1 = memory.full[indirect_address % 0x0100];
            byte2 = memory.full[(indirect_address + 1) % 0x0100];
            absolute_address = (byte2 << 8) | (byte1);
            PC += 1;
            break;
        case IndirectIndexed: // A little like Indirect_Y.
            // Take 1 byte, and take the address from there (with wraparound). Finally add Y.
            indirect_address = memory.full[PC];
            byte1 = memory.full[indirect_address % 0x0100];
            byte2 = memory.full[(indirect_address + 1) % 0x0100];
            absolute_address = ((byte2 << 8) | (byte1)) + Y;
            // Add extra cycle since it crosses a page boundary.
            if ((absolute_address & 0xFF00) != (byte2 << 8)) {
                cycles += 1;
            }
            PC += 1;
            break;
        
        case Implied:
            // Does not require any address in memory.
            absolute_address = 0x0000;
            break;
        case Accumulator:
            // Uses only the accumulator, no address necessary.
            absolute_address = 0x0000;
            break;
        case Immediate:
            // Uses supplied byte(s) as a constant.
            absolute_address = PC;
            break;
        case Relative:
            // Adds an offset to the PC if a condition is true.
            // The offset part is done in the branch instructions themselves.
            absolute_address = PC;
            break;
        default:
            break;
    }

    return 0;
}
