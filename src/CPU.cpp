#include "../include/CPU.h"


CPU::CPU() {
    fill(begin(memory), end(memory), 0);

    PC = 0x0000;
    SP = 0x00;

    accumulator = 0;

    X = 0;
    Y = 0;

    absolute_address = 0x0000;
    // TODO: CONTINUE FIXING THIS LOOKUP TABLE STUFF
    op_lookup = {
        // opcode / opFunction / opmode / opcycles
		{ 0x00, &CPU::BRK, IMP, 7 },{ 0x01, &CPU::ORA, IZX, 6 },{ 0x02, &CPU::UNK, IMP, 2 },{ 0x03, &CPU::UNK, IMP, 8 },{ 0x04, &CPU::NOP, ZPN, 3 },{ 0x05, &CPU::ORA, ZPN, 3 },{ 0x06, &CPU::ASL, ZPN, 5 },{ 0x07, &CPU::UNK, IMP, 5 },{ 0x08, &CPU::PHP, IMP, 3 },{ 0x09, &CPU::ORA, IMM, 2 },{ 0x0A, &CPU::ASL, IMP, 2 },{ 0x0B, &CPU::UNK, IMP, 2 },{ 0x0C, &CPU::NOP, ABS, 4 },{ 0x0D, &CPU::ORA, ABS, 4 },{ 0x0E, &CPU::ASL, ABS, 6 },{ 0x0F, &CPU::UNK, IMP, 6 },
		{ 0x10, &CPU::BPL, REL, 2 },{ 0x11, &CPU::ORA, IZY, 5 },{ 0x12, &CPU::UNK, IMP, 2 },{ 0x13, &CPU::UNK, IMP, 8 },{ 0x14, &CPU::NOP, ZPX, 4 },{ 0x15, &CPU::ORA, ZPX, 4 },{ 0x16, &CPU::ASL, ZPX, 6 },{ 0x17, &CPU::UNK, IMP, 6 },{ 0x18, &CPU::CLC, IMP, 2 },{ 0x19, &CPU::ORA, ABY, 4 },{ 0x1A, &CPU::NOP, IMP, 2 },{ 0x1B, &CPU::UNK, IMP, 7 },{ 0x1C, &CPU::NOP, ABX, 4 },{ 0x1D, &CPU::ORA, ABX, 4 },{ 0x1E, &CPU::ASL, ABX, 7 },{ 0x1F, &CPU::UNK, IMP, 7 },
		{ 0x20, &CPU::JSR, ABS, 6 },{ 0x21, &CPU::AND, IZX, 6 },{ 0x22, &CPU::UNK, IMP, 2 },{ 0x23, &CPU::UNK, IMP, 8 },{ 0x24, &CPU::BIT, ZPN, 3 },{ 0x25, &CPU::AND, ZPN, 3 },{ 0x26, &CPU::ROL, ZPN, 5 },{ 0x27, &CPU::UNK, IMP, 5 },{ 0x28, &CPU::PLP, IMP, 4 },{ 0x29, &CPU::AND, IMM, 2 },{ 0x2A, &CPU::ROL, IMP, 2 },{ 0x2B, &CPU::UNK, IMP, 2 },{ 0x2C, &CPU::BIT, ABS, 4 },{ 0x2D, &CPU::AND, ABS, 4 },{ 0x2E, &CPU::ROL, ABS, 6 },{ 0x2F, &CPU::UNK, IMP, 6 },
		{ 0x30, &CPU::BMI, REL, 2 },{ 0x31, &CPU::AND, IZY, 5 },{ 0x32, &CPU::UNK, IMP, 2 },{ 0x33, &CPU::UNK, IMP, 8 },{ 0x34, &CPU::NOP, ZPX, 4 },{ 0x35, &CPU::AND, ZPX, 4 },{ 0x36, &CPU::ROL, ZPX, 6 },{ 0x37, &CPU::UNK, IMP, 6 },{ 0x38, &CPU::SEC, IMP, 2 },{ 0x39, &CPU::AND, ABY, 4 },{ 0x3A, &CPU::NOP, IMP, 2 },{ 0x3B, &CPU::UNK, IMP, 7 },{ 0x3C, &CPU::NOP, ABX, 4 },{ 0x3D, &CPU::AND, ABX, 4 },{ 0x3E, &CPU::ROL, ABX, 7 },{ 0x3F, &CPU::UNK, IMP, 7 },
		{ 0x40, &CPU::RTI, IMP, 6 },{ 0x41, &CPU::EOR, IZX, 6 },{ 0x42, &CPU::UNK, IMP, 2 },{ 0x43, &CPU::UNK, IMP, 8 },{ 0x44, &CPU::NOP, ZPN, 3 },{ 0x45, &CPU::EOR, ZPN, 3 },{ 0x46, &CPU::LSR, ZPN, 5 },{ 0x47, &CPU::UNK, IMP, 5 },{ 0x48, &CPU::PHA, IMP, 3 },{ 0x49, &CPU::EOR, IMM, 2 },{ 0x4A, &CPU::LSR, IMP, 2 },{ 0x4B, &CPU::UNK, IMP, 2 },{ 0x4C, &CPU::JMP, ABS, 3 },{ 0x4D, &CPU::EOR, ABS, 4 },{ 0x4E, &CPU::LSR, ABS, 6 },{ 0x4F, &CPU::UNK, IMP, 6 },
		{ 0x50, &CPU::BVC, REL, 2 },{ 0x51, &CPU::EOR, IZY, 5 },{ 0x52, &CPU::UNK, IMP, 2 },{ 0x53, &CPU::UNK, IMP, 8 },{ 0x54, &CPU::NOP, ZPX, 4 },{ 0x55, &CPU::EOR, ZPX, 4 },{ 0x56, &CPU::LSR, ZPX, 6 },{ 0x57, &CPU::UNK, IMP, 6 },{ 0x58, &CPU::CLI, IMP, 2 },{ 0x59, &CPU::EOR, ABY, 4 },{ 0x5A, &CPU::NOP, IMP, 2 },{ 0x5B, &CPU::UNK, IMP, 7 },{ 0x5C, &CPU::NOP, ABX, 4 },{ 0x5D, &CPU::EOR, ABX, 4 },{ 0x5E, &CPU::LSR, ABX, 7 },{ 0x5F, &CPU::UNK, IMP, 7 },
		{ 0x60, &CPU::RTS, IMP, 6 },{ 0x61, &CPU::ADC, IZX, 6 },{ 0x62, &CPU::UNK, IMP, 2 },{ 0x63, &CPU::UNK, IMP, 8 },{ 0x64, &CPU::NOP, ZPN, 3 },{ 0x65, &CPU::ADC, ZPN, 3 },{ 0x66, &CPU::ROR, ZPN, 5 },{ 0x67, &CPU::UNK, IMP, 5 },{ 0x68, &CPU::PLA, IMP, 4 },{ 0x69, &CPU::ADC, IMM, 2 },{ 0x6A, &CPU::ROR, IMP, 2 },{ 0x6B, &CPU::UNK, IMP, 2 },{ 0x6C, &CPU::JMP, IND, 5 },{ 0x6D, &CPU::ADC, ABS, 4 },{ 0x6E, &CPU::ROR, ABS, 6 },{ 0x6F, &CPU::UNK, IMP, 6 },
		{ 0x70, &CPU::BVS, REL, 2 },{ 0x71, &CPU::ADC, IZY, 5 },{ 0x72, &CPU::UNK, IMP, 2 },{ 0x73, &CPU::UNK, IMP, 8 },{ 0x74, &CPU::NOP, ZPX, 4 },{ 0x75, &CPU::ADC, ZPX, 4 },{ 0x76, &CPU::ROR, ZPX, 6 },{ 0x77, &CPU::UNK, IMP, 6 },{ 0x78, &CPU::SEI, IMP, 2 },{ 0x79, &CPU::ADC, ABY, 4 },{ 0x7A, &CPU::NOP, IMP, 2 },{ 0x7B, &CPU::UNK, IMP, 7 },{ 0x7C, &CPU::NOP, ABX, 4 },{ 0x7D, &CPU::ADC, ABX, 4 },{ 0x7E, &CPU::ROR, ABX, 7 },{ 0x7F, &CPU::UNK, IMP, 7 },
		{ 0x80, &CPU::NOP, IMM, 2 },{ 0x81, &CPU::STA, IZX, 6 },{ 0x82, &CPU::NOP, IMM, 2 },{ 0x83, &CPU::UNK, IMP, 6 },{ 0x84, &CPU::STY, ZPN, 3 },{ 0x85, &CPU::STA, ZPN, 3 },{ 0x86, &CPU::STX, ZPN, 3 },{ 0x87, &CPU::UNK, IMP, 3 },{ 0x88, &CPU::DEY, IMP, 2 },{ 0x89, &CPU::NOP, IMM, 2 },{ 0x8A, &CPU::TXA, IMP, 2 },{ 0x8B, &CPU::UNK, IMP, 2 },{ 0x8C, &CPU::STY, ABS, 4 },{ 0x8D, &CPU::STA, ABS, 4 },{ 0x8E, &CPU::STX, ABS, 4 },{ 0x8F, &CPU::UNK, IMP, 4 },
		{ 0x90, &CPU::BCC, REL, 2 },{ 0x91, &CPU::STA, IZY, 6 },{ 0x92, &CPU::UNK, IMP, 2 },{ 0x93, &CPU::UNK, IMP, 6 },{ 0x94, &CPU::STY, ZPX, 4 },{ 0x95, &CPU::STA, ZPX, 4 },{ 0x96, &CPU::STX, ZPY, 4 },{ 0x97, &CPU::UNK, IMP, 4 },{ 0x98, &CPU::TYA, IMP, 2 },{ 0x99, &CPU::STA, ABY, 5 },{ 0x9A, &CPU::TXS, IMP, 2 },{ 0x9B, &CPU::UNK, IMP, 5 },{ 0x9C, &CPU::UNK, IMP, 5 },{ 0x9D, &CPU::STA, ABX, 5 },{ 0x9E, &CPU::UNK, IMP, 5 },{ 0x9F, &CPU::UNK, IMP, 5 },
		{ 0xA0, &CPU::LDY, IMM, 2 },{ 0xA1, &CPU::LDA, IZX, 6 },{ 0xA2, &CPU::LDX, IMM, 2 },{ 0xA3, &CPU::UNK, IMP, 6 },{ 0xA4, &CPU::LDY, ZPN, 3 },{ 0xA5, &CPU::LDA, ZPN, 3 },{ 0xA6, &CPU::LDX, ZPN, 3 },{ 0xA7, &CPU::UNK, IMP, 3 },{ 0xA8, &CPU::TAY, IMP, 2 },{ 0xA9, &CPU::LDA, IMM, 2 },{ 0xAA, &CPU::TAX, IMP, 2 },{ 0xAB, &CPU::UNK, IMP, 2 },{ 0xAC, &CPU::LDY, ABS, 4 },{ 0xAD, &CPU::LDA, ABS, 4 },{ 0xAE, &CPU::LDX, ABS, 4 },{ 0xAF, &CPU::UNK, IMP, 4 },
		{ 0xB0, &CPU::BCS, REL, 2 },{ 0xB1, &CPU::LDA, IZY, 5 },{ 0xB2, &CPU::UNK, IMP, 2 },{ 0xB3, &CPU::UNK, IMP, 5 },{ 0xB4, &CPU::LDY, ZPX, 4 },{ 0xB5, &CPU::LDA, ZPX, 4 },{ 0xB6, &CPU::LDX, ZPY, 4 },{ 0xB7, &CPU::UNK, IMP, 4 },{ 0xB8, &CPU::CLV, IMP, 2 },{ 0xB9, &CPU::LDA, ABY, 4 },{ 0xBA, &CPU::TSX, IMP, 2 },{ 0xBB, &CPU::UNK, IMP, 4 },{ 0xBC, &CPU::LDY, ABX, 4 },{ 0xBD, &CPU::LDA, ABX, 4 },{ 0xBE, &CPU::LDX, ABY, 4 },{ 0xBF, &CPU::UNK, IMP, 4 },
		{ 0xC0, &CPU::CPY, IMM, 2 },{ 0xC1, &CPU::CMP, IZX, 6 },{ 0xC2, &CPU::NOP, IMM, 2 },{ 0xC3, &CPU::UNK, IMP, 8 },{ 0xC4, &CPU::CPY, ZPN, 3 },{ 0xC5, &CPU::CMP, ZPN, 3 },{ 0xC6, &CPU::DEC, ZPN, 5 },{ 0xC7, &CPU::UNK, IMP, 5 },{ 0xC8, &CPU::INY, IMP, 2 },{ 0xC9, &CPU::CMP, IMM, 2 },{ 0xCA, &CPU::DEX, IMP, 2 },{ 0xCB, &CPU::UNK, IMP, 2 },{ 0xCC, &CPU::CPY, ABS, 4 },{ 0xCD, &CPU::CMP, ABS, 4 },{ 0xCE, &CPU::DEC, ABS, 6 },{ 0xCF, &CPU::UNK, IMP, 6 },
		{ 0xD0, &CPU::BNE, REL, 2 },{ 0xD1, &CPU::CMP, IZY, 5 },{ 0xD2, &CPU::UNK, IMP, 2 },{ 0xD3, &CPU::UNK, IMP, 8 },{ 0xD4, &CPU::NOP, ZPX, 4 },{ 0xD5, &CPU::CMP, ZPX, 4 },{ 0xD6, &CPU::DEC, ZPX, 6 },{ 0xD7, &CPU::UNK, IMP, 6 },{ 0xD8, &CPU::CLD, IMP, 2 },{ 0xD9, &CPU::CMP, ABY, 4 },{ 0xDA, &CPU::NOP, IMP, 2 },{ 0xDB, &CPU::UNK, IMP, 7 },{ 0xDC, &CPU::NOP, ABX, 4 },{ 0xDD, &CPU::CMP, ABX, 4 },{ 0xDE, &CPU::DEC, ABX, 7 },{ 0xDF, &CPU::UNK, IMP, 7 },
		{ 0xE0, &CPU::CPX, IMM, 2 },{ 0xE1, &CPU::SBC, IZX, 6 },{ 0xE2, &CPU::NOP, IMM, 2 },{ 0xE3, &CPU::UNK, IMP, 8 },{ 0xE4, &CPU::CPX, ZPN, 3 },{ 0xE5, &CPU::SBC, ZPN, 3 },{ 0xE6, &CPU::INC, ZPN, 5 },{ 0xE7, &CPU::UNK, IMP, 5 },{ 0xE8, &CPU::INX, IMP, 2 },{ 0xE9, &CPU::SBC, IMM, 2 },{ 0xEA, &CPU::NOP, IMP, 2 },{ 0xEB, &CPU::SBC, IMP, 2 },{ 0xEC, &CPU::CPX, ABS, 4 },{ 0xED, &CPU::SBC, ABS, 4 },{ 0xEE, &CPU::INC, ABS, 6 },{ 0xEF, &CPU::UNK, IMP, 6 },
		{ 0xF0, &CPU::BEQ, REL, 2 },{ 0xF1, &CPU::SBC, IZY, 5 },{ 0xF2, &CPU::UNK, IMP, 2 },{ 0xF3, &CPU::UNK, IMP, 8 },{ 0xF4, &CPU::NOP, ZPX, 4 },{ 0xF5, &CPU::SBC, ZPX, 4 },{ 0xF6, &CPU::INC, ZPX, 6 },{ 0xF7, &CPU::UNK, IMP, 6 },{ 0xF8, &CPU::SED, IMP, 2 },{ 0xF9, &CPU::SBC, ABY, 4 },{ 0xFA, &CPU::NOP, IMP, 2 },{ 0xFB, &CPU::UNK, IMP, 7 },{ 0xFC, &CPU::NOP, ABX, 4 },{ 0xFD, &CPU::SBC, ABX, 4 },{ 0xFE, &CPU::INC, ABX, 7 },{ 0xFF, &CPU::UNK, IMP, 7 }
    };
}

CPU::~CPU() {
    // Destructor.
}

bool CPU::executeCycle() {
    // Execute a single cycle.
    if (cycles == 0) {
        // If opcode -> set mode -> set cycles -> call readaddress -> call opcode function.
        // TODO: FIX THE PRINTING OF OPCODES AND THE LOADING OF ROMS !!!
        opcode = memory[PC];
        // cout << "OPCODE: ";
        // cout << hex << opcode;
        // cout << endl;
        PC += 1;
        mode = op_lookup[opcode].opmode;
        cycles = op_lookup[opcode].opcycles;
        readAddress();
        (this->*op_lookup[opcode].opFunction)();
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
        case ZPN:
            // The absolute address is just 1 byte.
            byte1 = memory[PC];
            absolute_address = byte1;
            PC += 1;
            break;
        case ZPX:
            // The absolute address is 1 byte plus X.
            byte1 = memory[PC];
            absolute_address = (byte1 + X) % 0x0100;
            PC += 1;
            break;
        case ZPY:
            // The absolute address is 1 byte plus Y.
            byte1 = memory[PC];
            absolute_address = (byte1 + Y) % 0x0100;
            PC += 1;
            break;
        
        case ABS:
            // The absolute address is 2 bytes swapped around.
            byte1 = memory[PC];
            byte2 = memory[PC + 1];
            absolute_address = (byte2 << 8) | (byte1);
            PC += 2;
            break;
        case ABX:
            // The absolute address is 2 bytes swapped around plus X.
            byte1 = memory[PC];
            byte2 = memory[PC + 1];
            absolute_address = ((byte2 << 8) | (byte1)) + X;
            // Add extra cycle since it crosses a page boundary.
            if ((absolute_address & 0xFF00) != (byte2 << 8)) {
                cycles += 1;
            }
            PC += 2;
            break;
        case ABY:
            // The absolute address is 2 bytes swapped around plus Y.
            byte1 = memory[PC];
            byte2 = memory[PC + 1];
            absolute_address = ((byte2 << 8) | (byte1)) + Y;
            // Add extra cycle since it crosses a page boundary.
            if ((absolute_address & 0xFF00) != (byte2 << 8)) {
                cycles += 1;
            }
            PC += 2;
            break;
        
        case IND:
            // The absolute address is at the location of the 2 bytes swapped around.
            byte1 = memory[PC];
            byte2 = memory[PC + 1];
            indirect_address = (byte2 << 8) | (byte1);
            // A bug in the NES causes the +1 to not move to the next page when the lower byte is 0x00FF.
            if (byte1 == 0x00FF) {
                byte1 = memory[indirect_address];
                byte2 = memory[indirect_address & 0xFF00];
            }
            else {
                byte1 = memory[indirect_address];
                byte2 = memory[indirect_address + 1];
            }
            absolute_address = ((byte2 << 8) | (byte1)) + Y;
            PC += 2;
            break;
        case IZX:
            // Take 1 byte + X, and take the address from there (with wraparound).
            indirect_address = memory[PC] + X;
            byte1 = memory[indirect_address % 0x0100];
            byte2 = memory[(indirect_address + 1) % 0x0100];
            absolute_address = (byte2 << 8) | (byte1);
            PC += 1;
            break;
        case IZY: // A little like Indirect_Y.
            // Take 1 byte, and take the address from there (with wraparound). Finally add Y.
            indirect_address = memory[PC];
            byte1 = memory[indirect_address % 0x0100];
            byte2 = memory[(indirect_address + 1) % 0x0100];
            absolute_address = ((byte2 << 8) | (byte1)) + Y;
            // Add extra cycle since it crosses a page boundary.
            if ((absolute_address & 0xFF00) != (byte2 << 8)) {
                cycles += 1;
            }
            PC += 1;
            break;
        
        case IMP:
            // Does not require any address in memory.
            absolute_address = 0x0000;
            break;
        case ACC:
            // Uses only the accumulator, no address necessary.
            absolute_address = 0x0000;
            break;
        case IMM:
            // Uses supplied byte(s) as a constant.
            absolute_address = PC;
            break;
        case REL:
            // Adds an offset to the PC if a condition is true.
            // The offset part is done in the branch instructions themselves.
            absolute_address = PC;
            break;
        default:
            break;
    }

    return 0;
}


bool CPU::loadRom() {
    ifstream romFile("roms/nestest.nes", ios::in | ios::binary);
    int i = 0;
    unsigned char curr_char = 0;

    if (romFile.fail()) {
        cout << "Error: Could not open ROM file!\n";
        return 1;
    }

    romFile.seekg (0x0010);
    vector<char> prog((istreambuf_iterator<char>(romFile)), istreambuf_iterator<char>());

    copy(prog.begin(), prog.end(), &memory[0x8000]);
    // copy(prog.begin(), prog.end(), &memory[0xC000]);

    romFile.close();
    return 0;
}




/*
========================================================================
============================< INSTRUCTIONS >============================
========================================================================
*/

bool CPU::ADC() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.

    return 0;
}

bool CPU::AND() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::ASL() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::BCC() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::BCS() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::BEQ() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::BIT() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::BMI() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::BNE() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::BPL() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::BRK() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::BVC() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::BVS() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::CLC() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::CLD() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::CLI() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::CLV() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::CMP() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::CPX() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::CPY() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::DEC() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::DEX() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::DEY() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::EOR() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::INC() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::INX() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::INY() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::JMP() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::JSR() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::LDA() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::LDX() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::LDY() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::LSR() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::NOP() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::ORA() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::PHA() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::PHP() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::PLA() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::PLP() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::ROL() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::ROR() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::RTI() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::RTS() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::SBC() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::SEC() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::SED() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::SEI() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::STA() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::STX() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::STY() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::TAX() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::TAY() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::TSX() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::TXA() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::TXS() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::TYA() {
    // TODO: IMPLEMENT THIS AND EVERYTHING ELSE THAT REQUIRES SPECIFIC ADDRESSING MODES.
    return 0;
}

bool CPU::UNK() {
    cout << "Error: Unknown operation!";
    return 0;
}
