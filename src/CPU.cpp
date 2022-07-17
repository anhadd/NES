#include "../include/CPU.h"




CPU::CPU() {
    // Constructor.
    status.full = 0x00 | UNUSED_MASK | BREAK_COMMAND_MASK | INTERRUPT_DISABLED_MASK;
    PC = 0x0000;
    SP = 0x00;

    accumulator = 0;
    X = 0;
    Y = 0;

    execute_nmi = false;

    absolute_address = 0x0000;

    op_lookup = {
        // opcode / opFunction / opmode / opcycles / extra_cycle
		{ 0x00, "BRK", &CPU::BRK, IMP, 7, 0 },{ 0x01, "ORA", &CPU::ORA, IZX, 6, 0 },{ 0x02, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0x03, "SLO", &CPU::SLO, IZX, 8, 0 },{ 0x04, "NOP", &CPU::NOP, ZPN, 3, 0 },{ 0x05, "ORA", &CPU::ORA, ZPN, 3, 0 },{ 0x06, "ASL", &CPU::ASL, ZPN, 5, 0 },{ 0x07, "SLO", &CPU::SLO, ZPN, 5, 0 },{ 0x08, "PHP", &CPU::PHP, IMP, 3, 0 },{ 0x09, "ORA", &CPU::ORA, IMM, 2, 0 },{ 0x0A, "ASL", &CPU::ASL, ACC, 2, 0 },{ 0x0B, "ANC", &CPU::ANC, IMM, 2, 0 },{ 0x0C, "NOP", &CPU::NOP, ABS, 4, 0 },{ 0x0D, "ORA", &CPU::ORA, ABS, 4, 0 },{ 0x0E, "ASL", &CPU::ASL, ABS, 6, 0 },{ 0x0F, "SLO", &CPU::SLO, ABS, 6, 0 },
		{ 0x10, "BPL", &CPU::BPL, REL, 2, 1 },{ 0x11, "ORA", &CPU::ORA, IZY, 5, 1 },{ 0x12, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0x13, "SLO", &CPU::SLO, IZY, 8, 0 },{ 0x14, "NOP", &CPU::NOP, ZPX, 4, 0 },{ 0x15, "ORA", &CPU::ORA, ZPX, 4, 0 },{ 0x16, "ASL", &CPU::ASL, ZPX, 6, 0 },{ 0x17, "SLO", &CPU::SLO, ZPX, 6, 0 },{ 0x18, "CLC", &CPU::CLC, IMP, 2, 0 },{ 0x19, "ORA", &CPU::ORA, ABY, 4, 1 },{ 0x1A, "NOP", &CPU::NOP, IMP, 2, 0 },{ 0x1B, "SLO", &CPU::SLO, ABY, 7, 0 },{ 0x1C, "NOP", &CPU::NOP, ABX, 4, 1 },{ 0x1D, "ORA", &CPU::ORA, ABX, 4, 1 },{ 0x1E, "ASL", &CPU::ASL, ABX, 7, 0 },{ 0x1F, "SLO", &CPU::SLO, ABX, 7, 0 },
		{ 0x20, "JSR", &CPU::JSR, ABS, 6, 0 },{ 0x21, "AND", &CPU::AND, IZX, 6, 0 },{ 0x22, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0x23, "RLA", &CPU::RLA, IZX, 8, 0 },{ 0x24, "BIT", &CPU::BIT, ZPN, 3, 0 },{ 0x25, "AND", &CPU::AND, ZPN, 3, 0 },{ 0x26, "ROL", &CPU::ROL, ZPN, 5, 0 },{ 0x27, "RLA", &CPU::RLA, ZPN, 5, 0 },{ 0x28, "PLP", &CPU::PLP, IMP, 4, 0 },{ 0x29, "AND", &CPU::AND, IMM, 2, 0 },{ 0x2A, "ROL", &CPU::ROL, ACC, 2, 0 },{ 0x2B, "ANC", &CPU::ANC, IMM, 2, 0 },{ 0x2C, "BIT", &CPU::BIT, ABS, 4, 0 },{ 0x2D, "AND", &CPU::AND, ABS, 4, 0 },{ 0x2E, "ROL", &CPU::ROL, ABS, 6, 0 },{ 0x2F, "RLA", &CPU::RLA, ABS, 6, 0 },
		{ 0x30, "BMI", &CPU::BMI, REL, 2, 1 },{ 0x31, "AND", &CPU::AND, IZY, 5, 1 },{ 0x32, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0x33, "RLA", &CPU::RLA, IZY, 8, 0 },{ 0x34, "NOP", &CPU::NOP, ZPX, 4, 0 },{ 0x35, "AND", &CPU::AND, ZPX, 4, 0 },{ 0x36, "ROL", &CPU::ROL, ZPX, 6, 0 },{ 0x37, "RLA", &CPU::RLA, ZPX, 6, 0 },{ 0x38, "SEC", &CPU::SEC, IMP, 2, 0 },{ 0x39, "AND", &CPU::AND, ABY, 4, 1 },{ 0x3A, "NOP", &CPU::NOP, IMP, 2, 0 },{ 0x3B, "RLA", &CPU::RLA, ABY, 7, 0 },{ 0x3C, "NOP", &CPU::NOP, ABX, 4, 1 },{ 0x3D, "AND", &CPU::AND, ABX, 4, 1 },{ 0x3E, "ROL", &CPU::ROL, ABX, 7, 0 },{ 0x3F, "RLA", &CPU::RLA, ABX, 7, 0 },
		{ 0x40, "RTI", &CPU::RTI, IMP, 6, 0 },{ 0x41, "EOR", &CPU::EOR, IZX, 6, 0 },{ 0x42, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0x43, "SRE", &CPU::SRE, IZX, 8, 0 },{ 0x44, "NOP", &CPU::NOP, ZPN, 3, 0 },{ 0x45, "EOR", &CPU::EOR, ZPN, 3, 0 },{ 0x46, "LSR", &CPU::LSR, ZPN, 5, 0 },{ 0x47, "SRE", &CPU::SRE, ZPN, 5, 0 },{ 0x48, "PHA", &CPU::PHA, IMP, 3, 0 },{ 0x49, "EOR", &CPU::EOR, IMM, 2, 0 },{ 0x4A, "LSR", &CPU::LSR, ACC, 2, 0 },{ 0x4B, "ALR", &CPU::ALR, IMM, 2, 0 },{ 0x4C, "JMP", &CPU::JMP, ABS, 3, 0 },{ 0x4D, "EOR", &CPU::EOR, ABS, 4, 0 },{ 0x4E, "LSR", &CPU::LSR, ABS, 6, 0 },{ 0x4F, "SRE", &CPU::SRE, ABS, 6, 0 },
		{ 0x50, "BVC", &CPU::BVC, REL, 2, 1 },{ 0x51, "EOR", &CPU::EOR, IZY, 5, 1 },{ 0x52, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0x53, "SRE", &CPU::SRE, IZY, 8, 0 },{ 0x54, "NOP", &CPU::NOP, ZPX, 4, 0 },{ 0x55, "EOR", &CPU::EOR, ZPX, 4, 0 },{ 0x56, "LSR", &CPU::LSR, ZPX, 6, 0 },{ 0x57, "SRE", &CPU::SRE, ZPX, 6, 0 },{ 0x58, "CLI", &CPU::CLI, IMP, 2, 0 },{ 0x59, "EOR", &CPU::EOR, ABY, 4, 1 },{ 0x5A, "NOP", &CPU::NOP, IMP, 2, 0 },{ 0x5B, "SRE", &CPU::SRE, ABY, 7, 0 },{ 0x5C, "NOP", &CPU::NOP, ABX, 4, 1 },{ 0x5D, "EOR", &CPU::EOR, ABX, 4, 1 },{ 0x5E, "LSR", &CPU::LSR, ABX, 7, 0 },{ 0x5F, "SRE", &CPU::SRE, ABX, 7, 0 },
		{ 0x60, "RTS", &CPU::RTS, IMP, 6, 0 },{ 0x61, "ADC", &CPU::ADC, IZX, 6, 0 },{ 0x62, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0x63, "RRA", &CPU::RRA, IZX, 8, 0 },{ 0x64, "NOP", &CPU::NOP, ZPN, 3, 0 },{ 0x65, "ADC", &CPU::ADC, ZPN, 3, 0 },{ 0x66, "ROR", &CPU::ROR, ZPN, 5, 0 },{ 0x67, "RRA", &CPU::RRA, ZPN, 5, 0 },{ 0x68, "PLA", &CPU::PLA, IMP, 4, 0 },{ 0x69, "ADC", &CPU::ADC, IMM, 2, 0 },{ 0x6A, "ROR", &CPU::ROR, ACC, 2, 0 },{ 0x6B, "ARR", &CPU::ARR, IMM, 2, 0 },{ 0x6C, "JMP", &CPU::JMP, IND, 5, 0 },{ 0x6D, "ADC", &CPU::ADC, ABS, 4, 0 },{ 0x6E, "ROR", &CPU::ROR, ABS, 6, 0 },{ 0x6F, "RRA", &CPU::RRA, ABS, 6, 0 },
		{ 0x70, "BVS", &CPU::BVS, REL, 2, 1 },{ 0x71, "ADC", &CPU::ADC, IZY, 5, 1 },{ 0x72, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0x73, "RRA", &CPU::RRA, IZY, 8, 0 },{ 0x74, "NOP", &CPU::NOP, ZPX, 4, 0 },{ 0x75, "ADC", &CPU::ADC, ZPX, 4, 0 },{ 0x76, "ROR", &CPU::ROR, ZPX, 6, 0 },{ 0x77, "RRA", &CPU::RRA, ZPX, 6, 0 },{ 0x78, "SEI", &CPU::SEI, IMP, 2, 0 },{ 0x79, "ADC", &CPU::ADC, ABY, 4, 1 },{ 0x7A, "NOP", &CPU::NOP, IMP, 2, 0 },{ 0x7B, "RRA", &CPU::RRA, ABY, 7, 0 },{ 0x7C, "NOP", &CPU::NOP, ABX, 4, 1 },{ 0x7D, "ADC", &CPU::ADC, ABX, 4, 1 },{ 0x7E, "ROR", &CPU::ROR, ABX, 7, 0 },{ 0x7F, "RRA", &CPU::RRA, ABX, 7, 0 },
		{ 0x80, "NOP", &CPU::NOP, IMM, 2, 0 },{ 0x81, "STA", &CPU::STA, IZX, 6, 0 },{ 0x82, "NOP", &CPU::NOP, IMM, 2, 0 },{ 0x83, "SAX", &CPU::SAX, IZX, 6, 0 },{ 0x84, "STY", &CPU::STY, ZPN, 3, 0 },{ 0x85, "STA", &CPU::STA, ZPN, 3, 0 },{ 0x86, "STX", &CPU::STX, ZPN, 3, 0 },{ 0x87, "SAX", &CPU::SAX, ZPN, 3, 0 },{ 0x88, "DEY", &CPU::DEY, IMP, 2, 0 },{ 0x89, "NOP", &CPU::NOP, IMM, 2, 0 },{ 0x8A, "TXA", &CPU::TXA, IMP, 2, 0 },{ 0x8B, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0x8C, "STY", &CPU::STY, ABS, 4, 0 },{ 0x8D, "STA", &CPU::STA, ABS, 4, 0 },{ 0x8E, "STX", &CPU::STX, ABS, 4, 0 },{ 0x8F, "SAX", &CPU::SAX, ABS, 4, 0 },
		{ 0x90, "BCC", &CPU::BCC, REL, 2, 1 },{ 0x91, "STA", &CPU::STA, IZY, 6, 0 },{ 0x92, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0x93, "UNK", &CPU::UNK, IMP, 6, 0 },{ 0x94, "STY", &CPU::STY, ZPX, 4, 0 },{ 0x95, "STA", &CPU::STA, ZPX, 4, 0 },{ 0x96, "STX", &CPU::STX, ZPY, 4, 0 },{ 0x97, "SAX", &CPU::SAX, ZPY, 4, 0 },{ 0x98, "TYA", &CPU::TYA, IMP, 2, 0 },{ 0x99, "STA", &CPU::STA, ABY, 5, 0 },{ 0x9A, "TXS", &CPU::TXS, IMP, 2, 0 },{ 0x9B, "UNK", &CPU::UNK, IMP, 5, 0 },{ 0x9C, "SHY", &CPU::SHY, ABX, 5, 0 },{ 0x9D, "STA", &CPU::STA, ABX, 5, 0 },{ 0x9E, "SHX", &CPU::SHX, ABY, 5, 0 },{ 0x9F, "UNK", &CPU::UNK, IMP, 5, 0 },
		{ 0xA0, "LDY", &CPU::LDY, IMM, 2, 0 },{ 0xA1, "LDA", &CPU::LDA, IZX, 6, 0 },{ 0xA2, "LDX", &CPU::LDX, IMM, 2, 0 },{ 0xA3, "LAX", &CPU::LAX, IZX, 6, 0 },{ 0xA4, "LDY", &CPU::LDY, ZPN, 3, 0 },{ 0xA5, "LDA", &CPU::LDA, ZPN, 3, 0 },{ 0xA6, "LDX", &CPU::LDX, ZPN, 3, 0 },{ 0xA7, "LAX", &CPU::LAX, ZPN, 3, 0 },{ 0xA8, "TAY", &CPU::TAY, IMP, 2, 0 },{ 0xA9, "LDA", &CPU::LDA, IMM, 2, 0 },{ 0xAA, "TAX", &CPU::TAX, IMP, 2, 0 },{ 0xAB, "LAX", &CPU::LAX, IMM, 2, 0 },{ 0xAC, "LDY", &CPU::LDY, ABS, 4, 0 },{ 0xAD, "LDA", &CPU::LDA, ABS, 4, 0 },{ 0xAE, "LDX", &CPU::LDX, ABS, 4, 0 },{ 0xAF, "LAX", &CPU::LAX, ABS, 4, 0 },
		{ 0xB0, "BCS", &CPU::BCS, REL, 2, 1 },{ 0xB1, "LDA", &CPU::LDA, IZY, 5, 1 },{ 0xB2, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0xB3, "LAX", &CPU::LAX, IZY, 5, 1 },{ 0xB4, "LDY", &CPU::LDY, ZPX, 4, 0 },{ 0xB5, "LDA", &CPU::LDA, ZPX, 4, 0 },{ 0xB6, "LDX", &CPU::LDX, ZPY, 4, 0 },{ 0xB7, "LAX", &CPU::LAX, ZPY, 4, 0 },{ 0xB8, "CLV", &CPU::CLV, IMP, 2, 0 },{ 0xB9, "LDA", &CPU::LDA, ABY, 4, 1 },{ 0xBA, "TSX", &CPU::TSX, IMP, 2, 0 },{ 0xBB, "LAS", &CPU::LAS, ABY, 4, 1 },{ 0xBC, "LDY", &CPU::LDY, ABX, 4, 1 },{ 0xBD, "LDA", &CPU::LDA, ABX, 4, 1 },{ 0xBE, "LDX", &CPU::LDX, ABY, 4, 1 },{ 0xBF, "LAX", &CPU::LAX, ABY, 4, 1 },
		{ 0xC0, "CPY", &CPU::CPY, IMM, 2, 0 },{ 0xC1, "CMP", &CPU::CMP, IZX, 6, 0 },{ 0xC2, "NOP", &CPU::NOP, IMM, 2, 0 },{ 0xC3, "DCP", &CPU::DCP, IZX, 8, 0 },{ 0xC4, "CPY", &CPU::CPY, ZPN, 3, 0 },{ 0xC5, "CMP", &CPU::CMP, ZPN, 3, 0 },{ 0xC6, "DEC", &CPU::DEC, ZPN, 5, 0 },{ 0xC7, "DCP", &CPU::DCP, ZPN, 5, 0 },{ 0xC8, "INY", &CPU::INY, IMP, 2, 0 },{ 0xC9, "CMP", &CPU::CMP, IMM, 2, 0 },{ 0xCA, "DEX", &CPU::DEX, IMP, 2, 0 },{ 0xCB, "AXS", &CPU::AXS, IMM, 2, 0 },{ 0xCC, "CPY", &CPU::CPY, ABS, 4, 0 },{ 0xCD, "CMP", &CPU::CMP, ABS, 4, 0 },{ 0xCE, "DEC", &CPU::DEC, ABS, 6, 0 },{ 0xCF, "DCP", &CPU::DCP, ABS, 6, 0 },
		{ 0xD0, "BNE", &CPU::BNE, REL, 2, 1 },{ 0xD1, "CMP", &CPU::CMP, IZY, 5, 1 },{ 0xD2, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0xD3, "DCP", &CPU::DCP, IZY, 8, 0 },{ 0xD4, "NOP", &CPU::NOP, ZPX, 4, 0 },{ 0xD5, "CMP", &CPU::CMP, ZPX, 4, 0 },{ 0xD6, "DEC", &CPU::DEC, ZPX, 6, 0 },{ 0xD7, "DCP", &CPU::DCP, ZPX, 6, 0 },{ 0xD8, "CLD", &CPU::CLD, IMP, 2, 0 },{ 0xD9, "CMP", &CPU::CMP, ABY, 4, 1 },{ 0xDA, "NOP", &CPU::NOP, IMP, 2, 0 },{ 0xDB, "DCP", &CPU::DCP, ABY, 7, 0 },{ 0xDC, "NOP", &CPU::NOP, ABX, 4, 1 },{ 0xDD, "CMP", &CPU::CMP, ABX, 4, 1 },{ 0xDE, "DEC", &CPU::DEC, ABX, 7, 0 },{ 0xDF, "DCP", &CPU::DCP, ABX, 7, 0 },
		{ 0xE0, "CPX", &CPU::CPX, IMM, 2, 0 },{ 0xE1, "SBC", &CPU::SBC, IZX, 6, 0 },{ 0xE2, "NOP", &CPU::NOP, IMM, 2, 0 },{ 0xE3, "ISC", &CPU::ISC, IZX, 8, 0 },{ 0xE4, "CPX", &CPU::CPX, ZPN, 3, 0 },{ 0xE5, "SBC", &CPU::SBC, ZPN, 3, 0 },{ 0xE6, "INC", &CPU::INC, ZPN, 5, 0 },{ 0xE7, "ISC", &CPU::ISC, ZPN, 5, 0 },{ 0xE8, "INX", &CPU::INX, IMP, 2, 0 },{ 0xE9, "SBC", &CPU::SBC, IMM, 2, 0 },{ 0xEA, "NOP", &CPU::NOP, IMP, 2, 0 },{ 0xEB, "SBC", &CPU::SBC, IMM, 2, 0 },{ 0xEC, "CPX", &CPU::CPX, ABS, 4, 0 },{ 0xED, "SBC", &CPU::SBC, ABS, 4, 0 },{ 0xEE, "INC", &CPU::INC, ABS, 6, 0 },{ 0xEF, "ISC", &CPU::ISC, ABS, 6, 0 },
		{ 0xF0, "BEQ", &CPU::BEQ, REL, 2, 1 },{ 0xF1, "SBC", &CPU::SBC, IZY, 5, 1 },{ 0xF2, "UNK", &CPU::UNK, IMP, 2, 0 },{ 0xF3, "ISC", &CPU::ISC, IZY, 8, 0 },{ 0xF4, "NOP", &CPU::NOP, ZPX, 4, 0 },{ 0xF5, "SBC", &CPU::SBC, ZPX, 4, 0 },{ 0xF6, "INC", &CPU::INC, ZPX, 6, 0 },{ 0xF7, "ISC", &CPU::ISC, ZPX, 6, 0 },{ 0xF8, "SED", &CPU::SED, IMP, 2, 0 },{ 0xF9, "SBC", &CPU::SBC, ABY, 4, 1 },{ 0xFA, "NOP", &CPU::NOP, IMP, 2, 0 },{ 0xFB, "ISC", &CPU::ISC, ABY, 7, 0 },{ 0xFC, "NOP", &CPU::NOP, ABX, 4, 1 },{ 0xFD, "SBC", &CPU::SBC, ABX, 4, 1 },{ 0xFE, "INC", &CPU::INC, ABX, 7, 0 },{ 0xFF, "ISC", &CPU::ISC, ABX, 7, 0 }
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
    opcode = 0x00;
    execute_nmi = false;

    status.full |= INTERRUPT_DISABLED_MASK;
    decrementSP();
    decrementSP();
    decrementSP();

    temp = 0x0000;

    absolute_address = 0x0000;
    relative_offset = 0x00;
    total_cycles = 0;


    // Load the PC from a fixed area in memory.
    PC = (cpuRead(0xFFFD) << 8) | cpuRead(0xFFFC);

    cycles = 7;
    additional_cycle = false;
}

void CPU::IRQ() {
    if (!status.interrupt_disabled) {
        pushPCToStack();

        pushStatusToStack(false);
        status.interrupt_disabled = 1;

        PC = (cpuRead(0xFFFF) << 8) | cpuRead(0xFFFE);
        cycles = 7;
    }
}

void CPU::NMI() {
    pushPCToStack();

	pushStatusToStack(false);
    status.interrupt_disabled = 1;

	PC = (cpuRead(0xFFFB) << 8) | cpuRead(0xFFFA);
	cycles = 7;
}

bool CPU::executeCycle() {
    // Execute a single cycle.
    if (cycles == 0) {
        // Set current opcode.
        opcode = cpuRead(PC);
        PC += 1;
        // Set addressing mode.
        mode = op_lookup[opcode].opmode;
        // Set required cycles.
        cycles = op_lookup[opcode].opcycles;
        additional_cycle = op_lookup[opcode].extra_cycle;
        // Get the absolute address.
        readAddress();
        // Call opcode function.
        (this->*op_lookup[opcode].opFunction)();
    }
    // Decrement the amount of cycles until an instruction is finished.
    cycles -= 1;
    total_cycles += 1;

    // If the instruction is finished and there is a pending NMI, execute the NMI.
    if (cycles == 0 && execute_nmi) {
        NMI();
        execute_nmi = false;
    }
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
            absolute_address = ((uint16_t)byte2 << 8) | ((uint16_t)byte1);
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
        case IZY: 
            // Can also be called Indirect_Y.
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
void CPU::checkBranch(bool flag) {
    // If the branch condition holds.
    if (flag) {
        // Calculate the new PC.
        temp = PC + relative_offset;
        // If a page is crossed add an extra instruction cycle.
        if ((temp & 0xFF00) != (PC & 0xFF00)) {
            cycles += 1;
        }
        // Set the new PC and add an extra instruction cycle for taking the branch.
        PC = temp;
        cycles += 1;
    }
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

// Increments the stack pointer (SP).
void CPU::incrementSP() {
    if (SP == 0xFF) {
        SP = 0x00;
    }
    else {
        SP += 1;
    }
}

// Pushes PC to Stack.
void CPU::pushPCToStack() {
    cpuWrite(SP + STACK_START, (PC >> 8) & 0x00FF);
    decrementSP();
    cpuWrite(SP + STACK_START, PC & 0x00FF);
    decrementSP();
}

// Pushes status register to Stack.
void CPU::pushStatusToStack(bool is_instruction) {
    if(is_instruction) {
        cpuWrite(SP + STACK_START, status.full | UNUSED_MASK | BREAK_COMMAND_MASK);
    }
    else {
        cpuWrite(SP + STACK_START, status.full | UNUSED_MASK);
    }
    decrementSP();
}



/*
========================================================================
============================< INSTRUCTIONS >============================
========================================================================

The information for the official instructions is not difficult to find.
If there is a special scenario that is not obvious there will be comment pointing it out.

The unofficial (illegal) instructions are more difficult to find information about.
By far most released games do not use these instructions, but test ROMs might.
Adding them will also put the emulator a step closer to being complete, so it is recommended
to add them, even if they do not work perfectly.

Amazing sources for instruction information:
----- Official Instructions Only -----
http://www.6502.org/tutorials/6502opcodes.html#ASL
https://www.middle-engine.com/blog/posts/2020/06/23/programming-the-nes-the-6502-in-detail
http://archive.6502.org/datasheets/rockwell_r650x_r651x.pdf

----- Official and Unofficial Instructions -----
http://www.oxyron.de/html/opcodes02.html
https://www.masswerk.at/6502/6502_instruction_set.html#SRE
*/

// Addition.
void CPU::ADC() {
    read_data = cpuRead(absolute_address);
    temp = (uint16_t)accumulator + (uint16_t)read_data + (uint16_t)status.carry;
    
    status.carry =      temp > 0xFF;
    // The overflow is decided by checking whether the sign of the number has changed.
    // If the sign before the instruction is either - or + for both operands, then the
    // result should match that. If it does not then there has been an overflow.
    status.overflow =   ((accumulator & NEGATIVE_MASK) == (read_data & NEGATIVE_MASK))
                        && ((temp & NEGATIVE_MASK) != (read_data & NEGATIVE_MASK));
    status.zero =       (temp & 0x00FF) == 0;
    status.negative =   (temp & NEGATIVE_MASK) != 0;

    accumulator = temp & 0x00FF;
}

// Bitwise and.
void CPU::AND() {
    accumulator &= cpuRead(absolute_address);
    
    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
}

// Shift left.
void CPU::ASL() {
    if (mode == ACC) {
        temp = (uint16_t)accumulator << 1;
        accumulator = temp & 0x00FF;
    }
    else {
        // Writes the original value once, then the updated value (Dummy write).
        read_data = cpuRead(absolute_address);
        cpuWrite(absolute_address, read_data);
        temp = (uint16_t)read_data << 1;
        cpuWrite(absolute_address, temp & 0x00FF);
    }
    
    status.carry =      temp > 0xFF;
    status.zero =       (temp & 0x00FF) == 0;
    status.negative =   (temp & NEGATIVE_MASK) != 0;
}

// Branch if carry flag is clear (Branch on Carry Clear).
void CPU::BCC() {
    checkBranch(!status.carry);
}

// Branch if carry flag is set (Branch on Carry Set).
void CPU::BCS() {
    checkBranch(status.carry);
}

// Branch if zero flag is set.
void CPU::BEQ() {
    checkBranch(status.zero);
}

// Test the bit in memory using the accumulator.
void CPU::BIT() {
    read_data = cpuRead(absolute_address);
    temp = (uint16_t)accumulator & (uint16_t)read_data;

    status.zero =       (temp & 0x00FF) == 0;
    status.negative =   (read_data & NEGATIVE_MASK) != 0;
    status.overflow =   (read_data & OVERFLOW_MASK) != 0;
}

// Branch if negative flag is set.
void CPU::BMI() {
    checkBranch(status.negative);
}

// Branch if zero flag is clear.
void CPU::BNE() {
    checkBranch(!status.zero);
}

// Branch if negative flag is clear.
void CPU::BPL() {
    checkBranch(!status.negative);
}

// Break.
void CPU::BRK() {
    // Dummy read.
    cpuRead(PC);

    // Pushes the PC+1 onto the stack.
    PC += 1;
    pushPCToStack();

    pushStatusToStack(true);
    status.interrupt_disabled = 1;

    PC = (cpuRead(0xFFFF) << 8) | cpuRead(0xFFFE);
}

// Branch if overflow flag is clear (Branch on V Clear).
void CPU::BVC() {
    checkBranch(!status.overflow);
}

// Branch if overflow flag is set (Branch on V Set).
void CPU::BVS() {
    checkBranch(status.overflow);
}

// Clear carry flag.
void CPU::CLC() {
    status.carry = 0;
}

// Clear decimal mode flag.
void CPU::CLD() {
    status.decimal_mode = 0;
}

// Clear interrupt disabled flag.
void CPU::CLI() {
    status.interrupt_disabled = 0;
}

// Clear overflow flag.
void CPU::CLV() {
    status.overflow = 0;
}

// Compare data in memory to the accumulator.
void CPU::CMP() {
    read_data = cpuRead(absolute_address);
    temp = (uint16_t)accumulator - (uint16_t)read_data;

    status.carry =      accumulator >= read_data;
    status.zero =       accumulator == read_data;
    status.negative =   (temp & NEGATIVE_MASK) != 0;
}

// Compare data in memory to X.
void CPU::CPX() {
    read_data = cpuRead(absolute_address);
    temp = (uint16_t)X - (uint16_t)read_data;

    status.carry =      X >= read_data;
    status.zero =       X == read_data;
    status.negative =   (temp & NEGATIVE_MASK) != 0;
}

// Compare data in memory to Y.
void CPU::CPY() {
    read_data = cpuRead(absolute_address);
    temp = (uint16_t)Y - (uint16_t)read_data;

    status.carry =      Y >= read_data;
    status.zero =       Y == read_data;
    status.negative =   (temp & NEGATIVE_MASK) != 0;
}

// Decrement value in memory.
void CPU::DEC() {
    // Writes the original value once, then the updated value (Dummy write).
    read_data = cpuRead(absolute_address);
    cpuWrite(absolute_address, read_data);

    if (read_data == 0) {
        cpuWrite(absolute_address, 0xFF);
        read_data = 0xFF;
    }
    else {
        cpuWrite(absolute_address, read_data - 1);
        read_data -= 1;
    }

    status.zero =       read_data == 0;
    status.negative =   (read_data & NEGATIVE_MASK) != 0;
}

// Decrement X
void CPU::DEX() {
    // If X would go below 0 it wraps around to 255.
    // This is jst to make it clear what happens, since it would happen automatically for uint_8 anyway.
    if (X == 0) {
        X = 0xFF;
    }
    else {
        X -= 1;
    }

    status.zero =       X == 0;
    status.negative =   (X & NEGATIVE_MASK) != 0;
}

// Decrement Y
void CPU::DEY() {
    // If Y would go below 0 it wraps around to 255.
    // This is jst to make it clear what happens, since it would happen automatically for uint_8 anyway.
    if (Y == 0) {
        Y = 0xFF;
    }
    else {
        Y -= 1;
    }

    status.zero =       Y == 0;
    status.negative =   (Y & NEGATIVE_MASK) != 0;
}

// Bitwise Exclusive OR.
void CPU::EOR() {
    accumulator ^= cpuRead(absolute_address);

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
}

// Increment value in memory.
void CPU::INC() {
    // Writes the original value once, then the updated value.
    read_data = cpuRead(absolute_address);
    cpuWrite(absolute_address, read_data);

    if (read_data == 0xFF) {
        cpuWrite(absolute_address, 0);
        read_data = 0;
    }
    else {
        cpuWrite(absolute_address, read_data + 1);
        read_data += 1;
    }

    status.zero =       read_data == 0;
    status.negative =   (read_data & NEGATIVE_MASK) != 0;
}

// Increment X.
void CPU::INX() {
    if (X == 0xFF) {
        X = 0;
    }
    else {
        X += 1;
    }

    status.zero =       X == 0;
    status.negative =   (X & NEGATIVE_MASK) != 0;
}

// Increment Y.
void CPU::INY() {
    if (Y == 0xFF) {
        Y = 0;
    }
    else {
        Y += 1;
    }

    status.zero =       Y == 0;
    status.negative =   (Y & NEGATIVE_MASK) != 0;
}

// Jump.
void CPU::JMP() {
    PC = absolute_address;
}

// Jump to Sub Routine.
void CPU::JSR() {
    PC -= 1;
    pushPCToStack();

    PC = absolute_address;
}

// Load value in memory into the accumulator.
void CPU::LDA() {
    accumulator = cpuRead(absolute_address);

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
}

// Load value in memory into X.
void CPU::LDX() {
    X = cpuRead(absolute_address);

    status.zero =       X == 0;
    status.negative =   (X & NEGATIVE_MASK) != 0;
}

// Load value in memory into Y.
void CPU::LDY() {
    Y = cpuRead(absolute_address);

    status.zero =       Y == 0;
    status.negative =   (Y & NEGATIVE_MASK) != 0;
}

// Shift right.
void CPU::LSR() {
    if (mode == ACC) {
        temp = accumulator >> 1;
        status.carry =      (accumulator & CARRY_MASK) != 0;
        accumulator = temp & 0x00FF;
    }
    else {
        // Writes the original value once, then the updated value.
        read_data = cpuRead(absolute_address);
        cpuWrite(absolute_address, read_data);

        temp = read_data >> 1;
        status.carry =      (read_data & CARRY_MASK) != 0;
        cpuWrite(absolute_address, temp & 0x00FF);
    }

    status.zero =       (temp & 0x00FF) == 0;
    status.negative =   (temp & NEGATIVE_MASK) != 0;
}

// Does nothing.
void CPU::NOP() {

}

// Bitwise OR.
void CPU::ORA() {
    accumulator |= cpuRead(absolute_address);

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
}

// Push accumulator onto stack.
void CPU::PHA() {
    cpuWrite(SP + STACK_START, accumulator);
    decrementSP();
}

// Push status register onto stack.
void CPU::PHP() {
    pushStatusToStack(true);
}

// Pull value from stack into the accumulator.
void CPU::PLA() {
    incrementSP();
    accumulator = cpuRead(SP + STACK_START);

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
}

// Pull value from stack into the status register.
void CPU::PLP() {
    incrementSP();
    status.full = cpuRead(SP + STACK_START) | UNUSED_MASK;
    status.break_command = 0;
}

// Rotate left. Incoming bit is the carry flag.
void CPU::ROL() {
    if (mode == ACC) {
        temp = ((uint16_t)accumulator << 1) | status.carry;
        status.carry =      (accumulator & NEGATIVE_MASK) != 0;

        accumulator = temp & 0x00FF;

        status.zero =       accumulator == 0;
        status.negative =   (accumulator & NEGATIVE_MASK) != 0;
    }
    else {
        // Writes the original value once, then the updated value (Dummy write).
        read_data = cpuRead(absolute_address);
        cpuWrite(absolute_address, read_data);

        temp = ((uint16_t)read_data << 1) | status.carry;
        status.carry =      (read_data & NEGATIVE_MASK) != 0;

        cpuWrite(absolute_address, temp & 0x00FF);
        read_data = temp & 0x00FF;
        
        status.zero =       read_data == 0;
        status.negative =   (read_data & NEGATIVE_MASK) != 0;
    }
}

// Rotate right. Incoming bit is the carry flag.
void CPU::ROR() {
    if (mode == ACC) {
        temp = ((uint16_t)accumulator >> 1) | ((uint16_t)status.carry << 7);
        status.carry =      (accumulator & CARRY_MASK) != 0;

        accumulator = temp & 0x00FF;

        status.zero =       accumulator == 0;
        status.negative =   (accumulator & NEGATIVE_MASK) != 0;
    }
    else {
        // Writes the original value once, then the updated value (Dummy write).
        read_data = cpuRead(absolute_address);
        cpuWrite(absolute_address, read_data);

        temp = ((uint16_t)read_data >> 1) | ((uint16_t)status.carry << 7);
        status.carry =      (read_data & CARRY_MASK) != 0;

        cpuWrite(absolute_address, temp & 0x00FF);
        read_data = temp & 0x00FF;

        status.zero =       read_data == 0;
        status.negative =   (read_data & NEGATIVE_MASK) != 0;
    }
}

// Return from interrupt.
void CPU::RTI() {
    // Dummy read.
    cpuRead(PC);

    incrementSP();
    status.full = cpuRead(SP + STACK_START) | UNUSED_MASK;
    
    incrementSP();
    uint8_t byte1 = cpuRead(SP + STACK_START);
    incrementSP();
    uint8_t byte2 = cpuRead(SP + STACK_START);
    PC = (byte2 << 8) | byte1;
}

// Return from subroutine.
void CPU::RTS() {
    // Dummy read.
    cpuRead(PC);

    incrementSP();
    uint8_t byte1 = cpuRead(SP + STACK_START);
    incrementSP();
    uint8_t byte2 = cpuRead(SP + STACK_START);
    PC = (byte2 << 8) | byte1;
    PC += 1;
}

// Subtraction.
void CPU::SBC() {
    // Subtraction is the same as addition but with the bits of the read_data flipped (XOR 0xFF).
    read_data = cpuRead(absolute_address) ^ 0xFF;
    temp = (uint16_t)accumulator + (uint16_t)read_data + (uint16_t)status.carry;
    
    status.carry =      temp > 0xFF;
    // The overflow is decided by checking whether the sign of the number has changed.
    // If the sign before the instruction is either - or + for both operands, then the
    // result should match that. If it does not then there has been an overflow.
    status.overflow =   ((accumulator & NEGATIVE_MASK) == (read_data & NEGATIVE_MASK))
                        && ((temp & NEGATIVE_MASK) != (read_data & NEGATIVE_MASK));
    status.zero =       (temp & 0x00FF) == 0;
    status.negative =   (temp & NEGATIVE_MASK) != 0;

    accumulator = temp & 0x00FF;
}

// Set carry flag.
void CPU::SEC() {
    status.carry = 1;
}

// Set decimal mode flag.
void CPU::SED() {
    status.decimal_mode = 1;
}

// Set interrupt flag.
void CPU::SEI() {
    status.interrupt_disabled = 1;
}

// Store the accumulator in memory.
void CPU::STA() {
    cpuWrite(absolute_address, accumulator);
}

// Store X in memory.
void CPU::STX() {
    cpuWrite(absolute_address, X);
}

// Store Y in memory.
void CPU::STY() {
    cpuWrite(absolute_address, Y);
}

// Store (Transfer) the accumulator in X.
void CPU::TAX() {
    X = accumulator;

    status.zero =       X == 0;
    status.negative =   (X & NEGATIVE_MASK) != 0;
}

// Store (Transfer) the accumulator in Y.
void CPU::TAY() {
    Y = accumulator;

    status.zero =       Y == 0;
    status.negative =   (Y & NEGATIVE_MASK) != 0;
}

// Store (Transfer) the stack pointer in X.
void CPU::TSX() {
    X = SP;

    status.zero =       X == 0;
    status.negative =   (X & NEGATIVE_MASK) != 0;
}

// Store (Transfer) X in the accumulator.
void CPU::TXA() {
    accumulator = X;

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
}

// Store (Transfer) X in the stack pointer.
void CPU::TXS() {
    SP = X;
}

// Store (Transfer) Y in the accumulator.
void CPU::TYA() {
    accumulator = Y;

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;
}

// Unknown OPCode encountered.
void CPU::UNK() {
    fprintf(stderr, "Error: Unknown operation!\n");
    fprintf(stderr, "OPCODE: %02x     PC: %04x\n\n", opcode, PC);
    
    // Exit.
    printf("Exiting: Invalid opcode encountered!\n");
    exit(0);
}





/* ILLEGAL INSTRUCTIONS
   Illegal instructions are instructions that are not "officially" supported, however 
   these instructions still do stuff which could be used by ROMs. It is usually a good idea
   to implement these instructions, since that is how a real NES would behave as well.

   Some illegal instructions are just 2 official instructions after each other, and these are 
   not difficult to implement.
   However other illegal instructions have weird side effects / differences which need to be kept
   in mind.
   
   note: XAA and LAX are highly unstable and should not be used, so they are not implemented.
         AHX, SHY, SHX, and TAS are also unstable, so they might not be completely correct.
   (from: http://www.oxyron.de/html/opcodes02.html)
*/
void CPU::SRE() {
    LSR();
    EOR();
}

void CPU::ISC() {
    INC();
    SBC();
}

void CPU::LAX() {
    LDA();
    LDX();
}

void CPU::SAX() {
    cpuWrite(absolute_address, accumulator & X);
}

void CPU::DCP() {
    DEC();
    CMP();
}

void CPU::SLO() {
    ASL();
    ORA();
}

void CPU::RLA() {
    ROL();
    AND();
}

void CPU::RRA() {
    ROR();
    ADC();
}

void CPU::ANC() {
    AND();
    // Copies bit 7 to the carry flag (as if ASL happened).
    status.carry =      (accumulator & NEGATIVE_MASK) != 0;
}

void CPU::ALR() {
    AND();

    // LSR ACC
    temp = accumulator >> 1;
    status.carry =      (accumulator & CARRY_MASK) != 0;
    accumulator = temp & 0x00FF;

    status.zero =       (temp & 0x00FF) == 0;
    status.negative =   (temp & NEGATIVE_MASK) != 0;
}

void CPU::ARR() {
    AND();
    
    // ROR ACC
    temp = ((uint16_t)accumulator >> 1) | ((uint16_t)status.carry << 7);

    accumulator = temp & 0x00FF;

    status.zero =       accumulator == 0;
    status.negative =   (accumulator & NEGATIVE_MASK) != 0;

    status.carry =      (accumulator & 0x40) != 0;
    status.overflow =   ((accumulator & 0x40) ^ (accumulator & 0x20)) != 0;
}

void CPU::AXS() {
    read_data = cpuRead(absolute_address) ^ 0xFF;
    temp = (accumulator & X) + read_data;

    status.zero =       temp > 0xFF;
    status.negative =   (temp & NEGATIVE_MASK) != 0;
    status.carry =      (accumulator & X) >= read_data;
}

void CPU::SHX() {
    temp = X & ((absolute_address - Y) >> 8);
    cpuWrite(absolute_address, temp);
}

void CPU::SHY() {
    temp = Y & ((absolute_address - X) >> 8);
    cpuWrite(absolute_address, temp);
}

void CPU::LAS() {
    read_data = cpuRead(absolute_address);
    accumulator =   read_data & status.full;
    X =             read_data & status.full;
    status.full =   read_data & status.full;
}
