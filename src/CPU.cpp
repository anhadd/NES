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
}

bool CPU::ADC() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::AND() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::readAddress(enum addressing_mode mode) {
    // Reads an address from the PC into absolute_address depending on the mode.
    uint8_t byte1;
    uint8_t byte2;

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
            byte1 = memory.full[PC + 1];
            absolute_address = (byte2 << 8) | (byte1);
            PC += 2;
            break;
        case Absolute_X:
            // The absolute address is 2 bytes swapped around plus X.
            byte1 = memory.full[PC];
            byte1 = memory.full[PC + 1];
            absolute_address = ((byte2 << 8) | (byte1)) + X;
            PC += 2;
            break;
        case Absolute_Y:
            // The absolute address is 2 bytes swapped around plus Y.
            byte1 = memory.full[PC];
            byte1 = memory.full[PC + 1];
            absolute_address = ((byte2 << 8) | (byte1)) + Y;
            PC += 2;
            break;
        case Indirect:

        case Indirect_X:

        case Indirect_Y:

        case Implied:

        case Accumulator:

        case Immediate:

        case Relative:

        case IndirectIndexed:
            
        default:
            break;
    }

    return 0;
}
