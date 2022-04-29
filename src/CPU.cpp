#include "../include/CPU.h"


CPU::CPU() {
    status.full = 0x24;

    PC = 0x0000;
    SP = 0xFF;

    accumulator = 0;

    X = 0;
    Y = 0;

    absolute_address = 0x0000;
    op_lookup = {
        // opcode / opFunction / opmode / opcycles / extra_cycle
		{ 0x00, &CPU::BRK, IMP, 7, 0 },{ 0x01, &CPU::ORA, IZX, 6, 0 },{ 0x02, &CPU::UNK, IMP, 2, 0 },{ 0x03, &CPU::UNK, IMP, 8, 0 },{ 0x04, &CPU::NOP, ZPN, 3, 0 },{ 0x05, &CPU::ORA, ZPN, 3, 0 },{ 0x06, &CPU::ASL, ZPN, 5, 0 },{ 0x07, &CPU::UNK, IMP, 5, 0 },{ 0x08, &CPU::PHP, IMP, 3, 0 },{ 0x09, &CPU::ORA, IMM, 2, 0 },{ 0x0A, &CPU::ASL, ACC, 2, 0 },{ 0x0B, &CPU::UNK, IMP, 2, 0 },{ 0x0C, &CPU::NOP, ABS, 4, 0 },{ 0x0D, &CPU::ORA, ABS, 4, 0 },{ 0x0E, &CPU::ASL, ABS, 6, 0 },{ 0x0F, &CPU::UNK, IMP, 6, 0 },
		{ 0x10, &CPU::BPL, REL, 2, 1 },{ 0x11, &CPU::ORA, IZY, 5, 1 },{ 0x12, &CPU::UNK, IMP, 2, 0 },{ 0x13, &CPU::UNK, IMP, 8, 0 },{ 0x14, &CPU::NOP, ZPX, 4, 0 },{ 0x15, &CPU::ORA, ZPX, 4, 0 },{ 0x16, &CPU::ASL, ZPX, 6, 0 },{ 0x17, &CPU::UNK, IMP, 6, 0 },{ 0x18, &CPU::CLC, IMP, 2, 0 },{ 0x19, &CPU::ORA, ABY, 4, 1 },{ 0x1A, &CPU::NOP, IMP, 2, 0 },{ 0x1B, &CPU::UNK, IMP, 7, 0 },{ 0x1C, &CPU::NOP, ABX, 4, 1 },{ 0x1D, &CPU::ORA, ABX, 4, 1 },{ 0x1E, &CPU::ASL, ABX, 7, 0 },{ 0x1F, &CPU::UNK, IMP, 7, 0 },
		{ 0x20, &CPU::JSR, ABS, 6, 0 },{ 0x21, &CPU::AND, IZX, 6, 0 },{ 0x22, &CPU::UNK, IMP, 2, 0 },{ 0x23, &CPU::UNK, IMP, 8, 0 },{ 0x24, &CPU::BIT, ZPN, 3, 0 },{ 0x25, &CPU::AND, ZPN, 3, 0 },{ 0x26, &CPU::ROL, ZPN, 5, 0 },{ 0x27, &CPU::UNK, IMP, 5, 0 },{ 0x28, &CPU::PLP, IMP, 4, 0 },{ 0x29, &CPU::AND, IMM, 2, 0 },{ 0x2A, &CPU::ROL, ACC, 2, 0 },{ 0x2B, &CPU::UNK, IMP, 2, 0 },{ 0x2C, &CPU::BIT, ABS, 4, 0 },{ 0x2D, &CPU::AND, ABS, 4, 0 },{ 0x2E, &CPU::ROL, ABS, 6, 0 },{ 0x2F, &CPU::UNK, IMP, 6, 0 },
		{ 0x30, &CPU::BMI, REL, 2, 1 },{ 0x31, &CPU::AND, IZY, 5, 1 },{ 0x32, &CPU::UNK, IMP, 2, 0 },{ 0x33, &CPU::UNK, IMP, 8, 0 },{ 0x34, &CPU::NOP, ZPX, 4, 0 },{ 0x35, &CPU::AND, ZPX, 4, 0 },{ 0x36, &CPU::ROL, ZPX, 6, 0 },{ 0x37, &CPU::UNK, IMP, 6, 0 },{ 0x38, &CPU::SEC, IMP, 2, 0 },{ 0x39, &CPU::AND, ABY, 4, 1 },{ 0x3A, &CPU::NOP, IMP, 2, 0 },{ 0x3B, &CPU::UNK, IMP, 7, 0 },{ 0x3C, &CPU::NOP, ABX, 4, 1 },{ 0x3D, &CPU::AND, ABX, 4, 1 },{ 0x3E, &CPU::ROL, ABX, 7, 0 },{ 0x3F, &CPU::UNK, IMP, 7, 0 },
		{ 0x40, &CPU::RTI, IMP, 6, 0 },{ 0x41, &CPU::EOR, IZX, 6, 0 },{ 0x42, &CPU::UNK, IMP, 2, 0 },{ 0x43, &CPU::UNK, IMP, 8, 0 },{ 0x44, &CPU::NOP, ZPN, 3, 0 },{ 0x45, &CPU::EOR, ZPN, 3, 0 },{ 0x46, &CPU::LSR, ZPN, 5, 0 },{ 0x47, &CPU::UNK, IMP, 5, 0 },{ 0x48, &CPU::PHA, IMP, 3, 0 },{ 0x49, &CPU::EOR, IMM, 2, 0 },{ 0x4A, &CPU::LSR, ACC, 2, 0 },{ 0x4B, &CPU::UNK, IMP, 2, 0 },{ 0x4C, &CPU::JMP, ABS, 3, 0 },{ 0x4D, &CPU::EOR, ABS, 4, 0 },{ 0x4E, &CPU::LSR, ABS, 6, 0 },{ 0x4F, &CPU::UNK, IMP, 6, 0 },
		{ 0x50, &CPU::BVC, REL, 2, 1 },{ 0x51, &CPU::EOR, IZY, 5, 1 },{ 0x52, &CPU::UNK, IMP, 2, 0 },{ 0x53, &CPU::UNK, IMP, 8, 0 },{ 0x54, &CPU::NOP, ZPX, 4, 0 },{ 0x55, &CPU::EOR, ZPX, 4, 0 },{ 0x56, &CPU::LSR, ZPX, 6, 0 },{ 0x57, &CPU::UNK, IMP, 6, 0 },{ 0x58, &CPU::CLI, IMP, 2, 0 },{ 0x59, &CPU::EOR, ABY, 4, 1 },{ 0x5A, &CPU::NOP, IMP, 2, 0 },{ 0x5B, &CPU::UNK, IMP, 7, 0 },{ 0x5C, &CPU::NOP, ABX, 4, 1 },{ 0x5D, &CPU::EOR, ABX, 4, 1 },{ 0x5E, &CPU::LSR, ABX, 7, 0 },{ 0x5F, &CPU::UNK, IMP, 7, 0 },
		{ 0x60, &CPU::RTS, IMP, 6, 0 },{ 0x61, &CPU::ADC, IZX, 6, 0 },{ 0x62, &CPU::UNK, IMP, 2, 0 },{ 0x63, &CPU::UNK, IMP, 8, 0 },{ 0x64, &CPU::NOP, ZPN, 3, 0 },{ 0x65, &CPU::ADC, ZPN, 3, 0 },{ 0x66, &CPU::ROR, ZPN, 5, 0 },{ 0x67, &CPU::UNK, IMP, 5, 0 },{ 0x68, &CPU::PLA, IMP, 4, 0 },{ 0x69, &CPU::ADC, IMM, 2, 0 },{ 0x6A, &CPU::ROR, ACC, 2, 0 },{ 0x6B, &CPU::UNK, IMP, 2, 0 },{ 0x6C, &CPU::JMP, IND, 5, 0 },{ 0x6D, &CPU::ADC, ABS, 4, 0 },{ 0x6E, &CPU::ROR, ABS, 6, 0 },{ 0x6F, &CPU::UNK, IMP, 6, 0 },
		{ 0x70, &CPU::BVS, REL, 2, 1 },{ 0x71, &CPU::ADC, IZY, 5, 1 },{ 0x72, &CPU::UNK, IMP, 2, 0 },{ 0x73, &CPU::UNK, IMP, 8, 0 },{ 0x74, &CPU::NOP, ZPX, 4, 0 },{ 0x75, &CPU::ADC, ZPX, 4, 0 },{ 0x76, &CPU::ROR, ZPX, 6, 0 },{ 0x77, &CPU::UNK, IMP, 6, 0 },{ 0x78, &CPU::SEI, IMP, 2, 0 },{ 0x79, &CPU::ADC, ABY, 4, 1 },{ 0x7A, &CPU::NOP, IMP, 2, 0 },{ 0x7B, &CPU::UNK, IMP, 7, 0 },{ 0x7C, &CPU::NOP, ABX, 4, 1 },{ 0x7D, &CPU::ADC, ABX, 4, 1 },{ 0x7E, &CPU::ROR, ABX, 7, 0 },{ 0x7F, &CPU::UNK, IMP, 7, 0 },
		{ 0x80, &CPU::NOP, IMM, 2, 0 },{ 0x81, &CPU::STA, IZX, 6, 0 },{ 0x82, &CPU::NOP, IMM, 2, 0 },{ 0x83, &CPU::UNK, IMP, 6, 0 },{ 0x84, &CPU::STY, ZPN, 3, 0 },{ 0x85, &CPU::STA, ZPN, 3, 0 },{ 0x86, &CPU::STX, ZPN, 3, 0 },{ 0x87, &CPU::UNK, IMP, 3, 0 },{ 0x88, &CPU::DEY, IMP, 2, 0 },{ 0x89, &CPU::NOP, IMM, 2, 0 },{ 0x8A, &CPU::TXA, IMP, 2, 0 },{ 0x8B, &CPU::UNK, IMP, 2, 0 },{ 0x8C, &CPU::STY, ABS, 4, 0 },{ 0x8D, &CPU::STA, ABS, 4, 0 },{ 0x8E, &CPU::STX, ABS, 4, 0 },{ 0x8F, &CPU::UNK, IMP, 4, 0 },
		{ 0x90, &CPU::BCC, REL, 2, 1 },{ 0x91, &CPU::STA, IZY, 6, 0 },{ 0x92, &CPU::UNK, IMP, 2, 0 },{ 0x93, &CPU::UNK, IMP, 6, 0 },{ 0x94, &CPU::STY, ZPX, 4, 0 },{ 0x95, &CPU::STA, ZPX, 4, 0 },{ 0x96, &CPU::STX, ZPY, 4, 0 },{ 0x97, &CPU::UNK, IMP, 4, 0 },{ 0x98, &CPU::TYA, IMP, 2, 0 },{ 0x99, &CPU::STA, ABY, 5, 0 },{ 0x9A, &CPU::TXS, IMP, 2, 0 },{ 0x9B, &CPU::UNK, IMP, 5, 0 },{ 0x9C, &CPU::UNK, IMP, 5, 0 },{ 0x9D, &CPU::STA, ABX, 5, 0 },{ 0x9E, &CPU::UNK, IMP, 5, 0 },{ 0x9F, &CPU::UNK, IMP, 5, 0 },
		{ 0xA0, &CPU::LDY, IMM, 2, 0 },{ 0xA1, &CPU::LDA, IZX, 6, 0 },{ 0xA2, &CPU::LDX, IMM, 2, 0 },{ 0xA3, &CPU::UNK, IMP, 6, 0 },{ 0xA4, &CPU::LDY, ZPN, 3, 0 },{ 0xA5, &CPU::LDA, ZPN, 3, 0 },{ 0xA6, &CPU::LDX, ZPN, 3, 0 },{ 0xA7, &CPU::UNK, IMP, 3, 0 },{ 0xA8, &CPU::TAY, IMP, 2, 0 },{ 0xA9, &CPU::LDA, IMM, 2, 0 },{ 0xAA, &CPU::TAX, IMP, 2, 0 },{ 0xAB, &CPU::UNK, IMP, 2, 0 },{ 0xAC, &CPU::LDY, ABS, 4, 0 },{ 0xAD, &CPU::LDA, ABS, 4, 0 },{ 0xAE, &CPU::LDX, ABS, 4, 0 },{ 0xAF, &CPU::UNK, IMP, 4, 0 },
		{ 0xB0, &CPU::BCS, REL, 2, 1 },{ 0xB1, &CPU::LDA, IZY, 5, 1 },{ 0xB2, &CPU::UNK, IMP, 2, 0 },{ 0xB3, &CPU::UNK, IMP, 5, 1 },{ 0xB4, &CPU::LDY, ZPX, 4, 0 },{ 0xB5, &CPU::LDA, ZPX, 4, 0 },{ 0xB6, &CPU::LDX, ZPY, 4, 0 },{ 0xB7, &CPU::UNK, IMP, 4, 0 },{ 0xB8, &CPU::CLV, IMP, 2, 0 },{ 0xB9, &CPU::LDA, ABY, 4, 1 },{ 0xBA, &CPU::TSX, IMP, 2, 0 },{ 0xBB, &CPU::UNK, IMP, 4, 1 },{ 0xBC, &CPU::LDY, ABX, 4, 1 },{ 0xBD, &CPU::LDA, ABX, 4, 1 },{ 0xBE, &CPU::LDX, ABY, 4, 1 },{ 0xBF, &CPU::UNK, IMP, 4, 1 },
		{ 0xC0, &CPU::CPY, IMM, 2, 0 },{ 0xC1, &CPU::CMP, IZX, 6, 0 },{ 0xC2, &CPU::NOP, IMM, 2, 0 },{ 0xC3, &CPU::UNK, IMP, 8, 0 },{ 0xC4, &CPU::CPY, ZPN, 3, 0 },{ 0xC5, &CPU::CMP, ZPN, 3, 0 },{ 0xC6, &CPU::DEC, ZPN, 5, 0 },{ 0xC7, &CPU::UNK, IMP, 5, 0 },{ 0xC8, &CPU::INY, IMP, 2, 0 },{ 0xC9, &CPU::CMP, IMM, 2, 0 },{ 0xCA, &CPU::DEX, IMP, 2, 0 },{ 0xCB, &CPU::UNK, IMP, 2, 0 },{ 0xCC, &CPU::CPY, ABS, 4, 0 },{ 0xCD, &CPU::CMP, ABS, 4, 0 },{ 0xCE, &CPU::DEC, ABS, 6, 0 },{ 0xCF, &CPU::UNK, IMP, 6, 0 },
		{ 0xD0, &CPU::BNE, REL, 2, 1 },{ 0xD1, &CPU::CMP, IZY, 5, 1 },{ 0xD2, &CPU::UNK, IMP, 2, 0 },{ 0xD3, &CPU::UNK, IMP, 8, 0 },{ 0xD4, &CPU::NOP, ZPX, 4, 0 },{ 0xD5, &CPU::CMP, ZPX, 4, 0 },{ 0xD6, &CPU::DEC, ZPX, 6, 0 },{ 0xD7, &CPU::UNK, IMP, 6, 0 },{ 0xD8, &CPU::CLD, IMP, 2, 0 },{ 0xD9, &CPU::CMP, ABY, 4, 1 },{ 0xDA, &CPU::NOP, IMP, 2, 0 },{ 0xDB, &CPU::UNK, IMP, 7, 0 },{ 0xDC, &CPU::NOP, ABX, 4, 1 },{ 0xDD, &CPU::CMP, ABX, 4, 1 },{ 0xDE, &CPU::DEC, ABX, 7, 0 },{ 0xDF, &CPU::UNK, IMP, 7, 0 },
		{ 0xE0, &CPU::CPX, IMM, 2, 0 },{ 0xE1, &CPU::SBC, IZX, 6, 0 },{ 0xE2, &CPU::NOP, IMM, 2, 0 },{ 0xE3, &CPU::UNK, IMP, 8, 0 },{ 0xE4, &CPU::CPX, ZPN, 3, 0 },{ 0xE5, &CPU::SBC, ZPN, 3, 0 },{ 0xE6, &CPU::INC, ZPN, 5, 0 },{ 0xE7, &CPU::UNK, IMP, 5, 0 },{ 0xE8, &CPU::INX, IMP, 2, 0 },{ 0xE9, &CPU::SBC, IMM, 2, 0 },{ 0xEA, &CPU::NOP, IMP, 2, 0 },{ 0xEB, &CPU::SBC, IMP, 2, 0 },{ 0xEC, &CPU::CPX, ABS, 4, 0 },{ 0xED, &CPU::SBC, ABS, 4, 0 },{ 0xEE, &CPU::INC, ABS, 6, 0 },{ 0xEF, &CPU::UNK, IMP, 6, 0 },
		{ 0xF0, &CPU::BEQ, REL, 2, 1 },{ 0xF1, &CPU::SBC, IZY, 5, 1 },{ 0xF2, &CPU::UNK, IMP, 2, 0 },{ 0xF3, &CPU::UNK, IMP, 8, 0 },{ 0xF4, &CPU::NOP, ZPX, 4, 0 },{ 0xF5, &CPU::SBC, ZPX, 4, 0 },{ 0xF6, &CPU::INC, ZPX, 6, 0 },{ 0xF7, &CPU::UNK, IMP, 6, 0 },{ 0xF8, &CPU::SED, IMP, 2, 0 },{ 0xF9, &CPU::SBC, ABY, 4, 1 },{ 0xFA, &CPU::NOP, IMP, 2, 0 },{ 0xFB, &CPU::UNK, IMP, 7, 0 },{ 0xFC, &CPU::NOP, ABX, 4, 1 },{ 0xFD, &CPU::SBC, ABX, 4, 1 },{ 0xFE, &CPU::INC, ABX, 7, 0 },{ 0xFF, &CPU::UNK, IMP, 7, 0 }
    };
}

CPU::~CPU() {
    // Destructor.
}

void CPU::passBUS(BUS* nesBUS) {
    bus = nesBUS;
}

uint8_t CPU::cpuRead(uint16_t address) {
    return bus->busReadCPU(address);
}

uint8_t CPU::cpuWrite(uint16_t address, uint8_t value) {
    bus->busWriteCPU(address, value);
    return 0;
}

void CPU::reset() {
    status.full = 0x24; // TODO: CHECK IF STATUS SHOULD BE RESET LIKE THIS
    SP = 0xFD;
    total_cycles = 7;

    // PC = 0xC000; // For nestest.
    PC = (cpuRead(0xFFFD) << 8) | cpuRead(0xFFFC); // For normal ROMs.
}

void CPU::IRQ() {
    if (!status.interrupt_disabled) {
        pushPCToStack();

        status.break_command = 0;
        status.full |= UNUSED_MASK | INTERRUPT_DISABLED_MASK;
        pushStatusToStack();

        PC = (cpuRead(0xFFFF) << 8) | cpuRead(0xFFFE);
        cycles = 7;
    }
}

void CPU::NMI() {
    pushPCToStack();

    status.break_command = 0;
    status.full |= UNUSED_MASK | INTERRUPT_DISABLED_MASK;
	pushStatusToStack();

	PC = (cpuRead(0xFFFB) << 8) | cpuRead(0xFFFA);
	cycles = 8;
}

bool CPU::executeCycle() {
    // Execute a single cycle.
    if (cycles == 0) {
        // Set opcode -> set mode -> set cycles -> call readaddress -> call opcode function.
        opcode = cpuRead(PC);
        fprintf(stderr, "%04x  %02x             A:%02x X:%02x Y:%02x P:%02x SP:%02x CYC:%u\n", PC, opcode, accumulator, X, Y, status.full, SP, total_cycles);

        PC += 1;
        mode = op_lookup[opcode].opmode;
        cycles = op_lookup[opcode].opcycles;
        additional_cycle = op_lookup[opcode].extra_cycle;
        readAddress();
        (this->*op_lookup[opcode].opFunction)();
    }
    cycles -= 1;
    total_cycles += 1;
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
            byte1 = cpuRead(PC);
            absolute_address = (uint16_t)byte1 & 0x00FF;
            PC += 1;
            break;
        case ZPX:
            // The absolute address is 1 byte plus X.
            byte1 = cpuRead(PC);
            absolute_address = ((uint16_t)byte1 + (uint16_t)X) % ZERO_PAGE_SIZE;
            PC += 1;
            break;
        case ZPY:
            // The absolute address is 1 byte plus Y.
            byte1 = cpuRead(PC);
            absolute_address = ((uint16_t)byte1 + (uint16_t)Y) % ZERO_PAGE_SIZE;
            PC += 1;
            break;
        
        case ABS:
            // The absolute address is 2 bytes swapped around.
            byte1 = cpuRead(PC);
            byte2 = cpuRead(PC + 1);
            absolute_address = ((uint16_t)byte2 << 8) | ((uint16_t)byte1);
            PC += 2;
            break;
        case ABX:
            // The absolute address is 2 bytes swapped around plus X.
            byte1 = cpuRead(PC);
            byte2 = cpuRead(PC + 1);
            absolute_address = (((uint16_t)byte2 << 8) | ((uint16_t)byte1)) + (uint16_t)X;
            // Add extra cycle since it crosses a page boundary.
            if (additional_cycle && ((absolute_address & 0xFF00) != (byte2 << 8))) {
                cycles += 1;
            }
            PC += 2;
            break;
        case ABY:
            // The absolute address is 2 bytes swapped around plus Y.
            byte1 = cpuRead(PC);
            byte2 = cpuRead(PC + 1);
            absolute_address = (((uint16_t)byte2 << 8) | ((uint16_t)byte1)) + (uint16_t)Y;
            // Add extra cycle since it crosses a page boundary.
            if (additional_cycle && ((absolute_address & 0xFF00) != (byte2 << 8))) {
                cycles += 1;
            }
            PC += 2;
            break;
        
        case IND:
            // The absolute address is at the location of the 2 bytes swapped around.
            byte1 = cpuRead(PC);
            byte2 = cpuRead(PC + 1);
            indirect_address = (byte2 << 8) | (byte1);
            // A bug in the NES causes the +1 to not move to the next page when the lower byte is 0x00FF.
            if (byte1 == 0x00FF) {
                byte1 = cpuRead(indirect_address);
                byte2 = cpuRead(indirect_address & 0xFF00);
            }
            else {
                byte1 = cpuRead(indirect_address);
                byte2 = cpuRead(indirect_address + 1);
            }
            absolute_address = (((uint16_t)byte2 << 8) | ((uint16_t)byte1)) + (uint16_t)Y;
            PC += 2;
            break;
        case IZX:
            // Take 1 byte + X, and take the address from there (with wraparound).
            indirect_address = cpuRead(PC) + X;
            byte1 = cpuRead(indirect_address % ZERO_PAGE_SIZE);
            byte2 = cpuRead((indirect_address + 1) % ZERO_PAGE_SIZE);
            absolute_address = ((uint16_t)byte2 << 8) | ((uint16_t)byte1);
            PC += 1;
            break;
        case IZY: // A little like Indirect_Y.
            // Take 1 byte, and take the address from there (with wraparound). Finally add Y.
            indirect_address = cpuRead(PC);
            byte1 = cpuRead(indirect_address % ZERO_PAGE_SIZE);
            byte2 = cpuRead((indirect_address + 1) % ZERO_PAGE_SIZE);
            absolute_address = (((uint16_t)byte2 << 8) | ((uint16_t)byte1)) + (uint16_t)Y;
            // Add extra cycle since it crosses a page boundary.
            if (additional_cycle && ((absolute_address & 0xFF00) != (byte2 << 8))) {
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
            PC += 1;
            break;
        case REL:
            // Adds an offset to the PC if a condition is true.
            relative_offset = (int8_t)cpuRead(PC);
            PC += 1;
            break;
        default:
            break;
    }
    return 0;
}

// Helper function for branches.
bool CPU::checkBranch(bool flag) {
    if (flag) {
        temp = PC + relative_offset;
        if ((temp & 0xFF00) != (PC & 0xFF00)) {
            cycles += 1;
        }
        PC = temp;
        cycles += 1;
    }
    return 0;
}

// Decrements the stack pointer (SP).
void CPU::decrementSP() {
    if (SP == 0x00) {
        SP = 0xFF;
    }
    else {
        SP -= 1;
    }
}

// Decrements the stack pointer (SP).
void CPU::incrementSP() {
    if (SP == 0xFF) {
        SP = 0x00;
    }
    else {
        SP += 1;
    }
}

// Pushes PC to Stack.
bool CPU::pushPCToStack() {
    cpuWrite(SP + STACK_START, (PC >> 8) & 0x00FF);
    decrementSP();
    cpuWrite(SP + STACK_START, PC & 0x00FF);
    decrementSP();
    return 0;
}

// Pushes status register to Stack.
bool CPU::pushStatusToStack() {
    cpuWrite(SP + STACK_START, status.full | UNUSED_MASK | BREAK_COMMAND_MASK);
    decrementSP();
    return 0;
}

/*
========================================================================
============================< INSTRUCTIONS >============================
========================================================================
*/

bool CPU::ADC() {
    temp = (uint16_t)accumulator + (uint16_t)cpuRead(absolute_address) + (uint16_t)status.carry;
    
    status.carry =      temp > 0xFF;
    status.overflow =   ((accumulator & NEGATIVE_MASK) && (cpuRead(absolute_address) & NEGATIVE_MASK) && !(temp & NEGATIVE_MASK)) 
                    || (!(accumulator & NEGATIVE_MASK) && !(cpuRead(absolute_address) & NEGATIVE_MASK) && (temp & NEGATIVE_MASK));
    status.zero =       (temp & 0x00FF) == 0;
    status.negative =   (temp & NEGATIVE_MASK) != 0;

    accumulator = temp & 0x00FF;
    return 0;
}

bool CPU::AND() {
    accumulator &= cpuRead(absolute_address);
    
    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::ASL() {
    if (mode == ACC) {
        temp = (uint16_t)accumulator << 1;
    }
    else {
        temp = (uint16_t)cpuRead(absolute_address) << 1;
    }
    
    status.carry =      temp > 0xFF;
    status.zero =       (temp & 0x00FF) == 0;
    status.negative =   (temp & NEGATIVE_MASK) != 0;

    if (mode == ACC) {
        accumulator = temp & 0x00FF;
    }
    else {
        cpuWrite(absolute_address, temp & 0x00FF);
    }
    return 0;
}

bool CPU::BCC() {
    checkBranch(!status.carry);
    return 0;
}

bool CPU::BCS() {
    checkBranch(status.carry);
    return 0;
}

bool CPU::BEQ() {
    checkBranch(status.zero);
    return 0;
}

bool CPU::BIT() {
    temp = (uint16_t)accumulator & (uint16_t)cpuRead(absolute_address);

    status.zero =       (temp & 0x00FF) == 0;
    status.negative =   (cpuRead(absolute_address) & NEGATIVE_MASK) != 0;
    status.overflow =   (cpuRead(absolute_address) & OVERFLOW_MASK) != 0;

    return 0;
}

bool CPU::BMI() {
    checkBranch(status.negative);
    return 0;
}

bool CPU::BNE() {
    checkBranch(!status.zero);
    return 0;
}

bool CPU::BPL() {
    checkBranch(!status.negative);
    return 0;
}

bool CPU::BRK() {
    pushPCToStack();

    status.interrupt_disabled = 1;
    pushStatusToStack();

    PC = (cpuRead(0xFFFF) << 8) | cpuRead(0xFFFE);
    return 0;
}

bool CPU::BVC() {
    checkBranch(!status.overflow);
    return 0;
}

bool CPU::BVS() {
    checkBranch(status.overflow);
    return 0;
}

bool CPU::CLC() {
    status.carry = 0;
    return 0;
}

bool CPU::CLD() {
    status.decimal_mode = 0;
    return 0;
}

bool CPU::CLI() {
    status.interrupt_disabled = 0;
    return 0;
}

bool CPU::CLV() {
    status.overflow = 0;
    return 0;
}

bool CPU::CMP() {
    temp = (uint16_t)accumulator - (uint16_t)cpuRead(absolute_address);

    status.carry =      accumulator >= cpuRead(absolute_address);
    status.zero =       accumulator == cpuRead(absolute_address);
    status.negative =   (temp & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::CPX() {
    temp = (uint16_t)X - (uint16_t)cpuRead(absolute_address);

    status.carry =      X >= cpuRead(absolute_address);
    status.zero =       X == cpuRead(absolute_address);
    status.negative =   (temp & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::CPY() {
    temp = (uint16_t)Y - (uint16_t)cpuRead(absolute_address);

    status.carry =      Y >= cpuRead(absolute_address);
    status.zero =       Y == cpuRead(absolute_address);
    status.negative =   (temp & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::DEC() {
    if (cpuRead(absolute_address) == 0) {
        cpuWrite(absolute_address, 0xFF);
    }
    else {
        cpuWrite(absolute_address, cpuRead(absolute_address) - 1);
    }

    status.zero =       cpuRead(absolute_address) == 0;
    status.negative =   (cpuRead(absolute_address) & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::DEX() {
    if (X == 0) {
        X = 0xFF;
    }
    else {
        X -= 1;
    }

    status.zero =       X == 0;
    status.negative =   (X & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::DEY() {
    if (Y == 0) {
        Y = 0xFF;
    }
    else {
        Y -= 1;
    }

    status.zero =       Y == 0;
    status.negative =   (Y & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::EOR() {
    accumulator ^= cpuRead(absolute_address);

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::INC() {
    if (cpuRead(absolute_address) == 0xFF) {
        cpuWrite(absolute_address, 0);
    }
    else {
        cpuWrite(absolute_address, cpuRead(absolute_address) + 1);
    }

    status.zero =       cpuRead(absolute_address) == 0;
    status.negative =   (cpuRead(absolute_address) & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::INX() {
    if (X == 0xFF) {
        X = 0;
    }
    else {
        X += 1;
    }

    status.zero =       X == 0;
    status.negative =   (X & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::INY() {
    if (Y == 0xFF) {
        Y = 0;
    }
    else {
        Y += 1;
    }

    status.zero =       Y == 0;
    status.negative =   (Y & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::JMP() {
    PC = absolute_address;
    return 0;
}

bool CPU::JSR() {
    PC -= 1;
    pushPCToStack();

    PC = absolute_address;
    return 0;
}

bool CPU::LDA() {
    accumulator = cpuRead(absolute_address);

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::LDX() {
    X = cpuRead(absolute_address);

    status.zero =       X == 0;
    status.negative =   (X & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::LDY() {
    Y = cpuRead(absolute_address);

    status.zero =       Y == 0;
    status.negative =   (Y & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::LSR() {
    if (mode == ACC) {
        temp = accumulator >> 1;
        status.carry =      (accumulator & CARRY_MASK) != 0;
    }
    else {
        temp = cpuRead(absolute_address) >> 1;
        status.carry =      (cpuRead(absolute_address) & CARRY_MASK) != 0;
    }

    status.zero =       (temp & 0x00FF) == 0;
    status.negative =   (temp & NEGATIVE_MASK) != 0;

    if (mode == ACC) {
        accumulator = temp & 0x00FF;
    }
    else {
        cpuWrite(absolute_address, temp & 0x00FF);
    }
    return 0;
}

bool CPU::NOP() {
    // Does nothing.
    return 0;
}

bool CPU::ORA() {
    accumulator |= cpuRead(absolute_address);

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::PHA() {
    cpuWrite(SP + STACK_START, accumulator);
    decrementSP();
    return 0;
}

bool CPU::PHP() {
    pushStatusToStack();
    return 0;
}

bool CPU::PLA() {
    incrementSP();
    accumulator = cpuRead(SP + STACK_START);

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::PLP() {
    incrementSP();
    status.full = cpuRead(SP + STACK_START) | UNUSED_MASK;
    status.break_command = 0;
    return 0;
}

bool CPU::ROL() {
    if (mode == ACC) {
        temp = ((uint16_t)accumulator << 1) | status.carry;
        status.carry =      (accumulator & NEGATIVE_MASK) != 0;

        accumulator = temp & 0x00FF;

        status.zero =       accumulator == 0;
        status.negative =   (accumulator & NEGATIVE_MASK) != 0;
    }
    else {
        temp = ((uint16_t)cpuRead(absolute_address) << 1) | status.carry;
        status.carry =      (cpuRead(absolute_address) & NEGATIVE_MASK) != 0;

        cpuWrite(absolute_address, temp & 0x00FF);
        
        status.zero =       cpuRead(absolute_address) == 0;
        status.negative =   (cpuRead(absolute_address) & NEGATIVE_MASK) != 0;
    }
    return 0;
}

bool CPU::ROR() {
    if (mode == ACC) {
        temp = ((uint16_t)accumulator >> 1) | ((uint16_t)status.carry << 7);
        status.carry =      (accumulator & CARRY_MASK) != 0;

        accumulator = temp & 0x00FF;

        status.zero =       accumulator == 0;
        status.negative =   (accumulator & NEGATIVE_MASK) != 0;
    }
    else {
        temp = ((uint16_t)cpuRead(absolute_address) >> 1) | ((uint16_t)status.carry << 7);
        status.carry =      (cpuRead(absolute_address) & CARRY_MASK) != 0;

        cpuWrite(absolute_address, temp & 0x00FF);

        status.zero =       cpuRead(absolute_address) == 0;
        status.negative =   (cpuRead(absolute_address) & NEGATIVE_MASK) != 0;
    }
    return 0;
}

bool CPU::RTI() {
    incrementSP();
    // TODO: CHECK IF THE UNUSED FLAG SHOULD REALLY BE SET HERE!
    status.full = cpuRead(SP + STACK_START) | UNUSED_MASK;
    incrementSP();
    
    PC = (cpuRead((SP + 1) + STACK_START) << 8) | cpuRead(SP + STACK_START);
    incrementSP();
    return 0;
}

bool CPU::RTS() {
    incrementSP();
    PC = (cpuRead((SP + 1) + STACK_START) << 8) | cpuRead(SP + STACK_START);
    PC += 1;
    incrementSP();
    return 0;
}

bool CPU::SBC() {
    uint8_t inverted = cpuRead(absolute_address) ^ 0xFF;

    temp = (uint16_t)accumulator + (uint16_t)inverted + (uint16_t)status.carry;
    
    status.carry =      temp > 0xFF;
    status.overflow =   ((accumulator & NEGATIVE_MASK) && (inverted & NEGATIVE_MASK) && !(temp & NEGATIVE_MASK)) 
                    || (!(accumulator & NEGATIVE_MASK) && !(inverted & NEGATIVE_MASK) && (temp & NEGATIVE_MASK));
    status.zero =       (temp & 0x00FF) == 0;
    status.negative =   (temp & NEGATIVE_MASK) != 0;

    accumulator = temp & 0x00FF;
    return 0;
}

bool CPU::SEC() {
    status.carry = 1;
    return 0;
}

bool CPU::SED() {
    status.decimal_mode = 1;
    return 0;
}

bool CPU::SEI() {
    status.interrupt_disabled = 1;
    return 0;
}

bool CPU::STA() {
    cpuWrite(absolute_address, accumulator);
    return 0;
}

bool CPU::STX() {
    cpuWrite(absolute_address, X);
    return 0;
}

bool CPU::STY() {
    cpuWrite(absolute_address, Y);
    return 0;
}

bool CPU::TAX() {
    X = accumulator;

    status.zero =       X == 0;
    status.negative =   (X & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::TAY() {
    Y = accumulator;

    status.zero =       Y == 0;
    status.negative =   (Y & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::TSX() {
    X = SP;

    status.zero =       X == 0;
    status.negative =   (X & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::TXA() {
    accumulator = X;

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::TXS() {
    SP = X;
    return 0;
}

bool CPU::TYA() {
    accumulator = Y;

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
    return 0;
}

bool CPU::UNK() {
    fprintf(stderr, "Error: Unknown operation!\n");
    fprintf(stderr, "OPCODE: %02x     PC: %04x\n\n", opcode, PC);
    // exit(0); // TODO: REMOVE THIS EXIT ONCE TESTING IS DONE !!!
    return 0;
}
