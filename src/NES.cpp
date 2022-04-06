#include "../include/NES.h"



NES::NES() {
    // Constructor
    fill(begin(memory), end(memory), 0);
    fill(begin(stack), end(stack), 0);

    fill(begin(V), end(V), 0);
    fill(begin(keys), end(keys), 0);

    fill_n(&display[0][0], 64 * 32, 0);

    curr_op = 0;
    x = 0;
    y = 0;
    h = 0;
    toggled = false;
    incrementPC = true;
    drawFlag = false;

    I = 0;
    delay_timer = 0;
    sound_timer = 0;

    PC = 0x200;
    SP = 0;

    font_addr = 0x050;
    rom_addr = 0x200;

    for (int i = 0; i < 5*16; i++) {
        memory[font_addr+i] = fontset[i];
    }
}


void NES::executeCycle(bool &play_sound) {
    // Gets the current operation.
    incrementPC = true;
    curr_op = (memory[PC] << 8) | memory[(PC) + 1];
    // cout << "PC: " << PC << "       " << "CURR OP: " << hex << curr_op << endl;
    
    switch (curr_op & 0xF000) {
        case 0x0000:
            // Operation starts with a 0.
            switch (curr_op & 0x0FFF) {
                case 0x00E0:
                    // Clears the screen.
                    fill_n(&display[0][0], 64 * 32, 0);
                    break;
                case 0x00EE:
                    // Returns form subroutine.
                    PC = stack[SP];
                    SP -= 1;
                    incrementPC = false;
                    break;
                default:
                    // Calls machine code routine at address NNN (from 0x0NNN).
                    SP += 1;
                    stack[SP] = PC + 2;
                    PC = curr_op;
                    incrementPC = false;
                    break;
            }
            break;
        case 0x1000:
            // Jumps to address NNN (from 0x1NNN).
            PC = curr_op & 0x0FFF;
            incrementPC = false;
            break;
        case 0x2000:
            // Calls subroutine at NNN (from 0x2NNN).
            SP += 1;
            stack[SP] = PC + 2; // TODO: MAYBE + 2?
            PC = curr_op & 0x0FFF;
            incrementPC = false;
            break;
        case 0x3000:
            // Skips the next instruction if VX equals NN.
            if ((uint8_t)V[(curr_op & 0x0F00) >> 8] == (uint8_t)(curr_op & 0x00FF)) {
                PC += 2;
            }
            break;
        case 0x4000:
            // Skips the next instruction if VX does not equal NN.
            if (V[(curr_op & 0x0F00) >> 8] != (curr_op & 0x00FF)) {
                PC += 2;
            }
            break;
        case 0x5000:
            // Skips the next instruction if VX equals VY.
            if (V[(curr_op & 0x0F00) >> 8] == V[(curr_op & 0x00F0) >> 4]) {
                PC += 2;
            }
            break;
        case 0x6000:
            // Sets VX to NN.
            V[(curr_op & 0x0F00) >> 8] = curr_op & 0x00FF;
            break;
        case 0x7000:
            // Adds NN to VX.
            V[(curr_op & 0x0F00) >> 8] += curr_op & 0x00FF;
            break;
        case 0x8000:
            switch (curr_op & 0x000F) {
                case 0x0000:
                    // Sets VX to the value of VY.
                    V[(curr_op & 0x0F00) >> 8] = V[(curr_op & 0x00F0) >> 4];
                    break;
                case 0x0001:
                    // Sets VX to VX or VY.
                    V[(curr_op & 0x0F00) >> 8] |= V[(curr_op & 0x00F0) >> 4];
                    break;
                case 0x0002:
                    // Sets VX to VX and VY.
                    V[(curr_op & 0x0F00) >> 8] &= V[(curr_op & 0x00F0) >> 4];
                    break;
                case 0x0003:
                    // Sets VX to VX xor VY.
                    V[(curr_op & 0x0F00) >> 8] ^= V[(curr_op & 0x00F0) >> 4];
                    break;
                case 0x0004:
                    // Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there is not.
                    if ((uint8_t)V[(curr_op & 0x0F00) >> 8] > (uint8_t)(255 - V[(curr_op & 0x00F0) >> 4])) {
                        V[15] = 1;
                    }
                    else {
                        V[15] = 0;
                    }
                    V[(curr_op & 0x0F00) >> 8] += V[(curr_op & 0x00F0) >> 4];
                    break;
                case 0x0005:
                    // VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there is not.
                    if ((uint8_t)V[(curr_op & 0x0F00) >> 8] >= (uint8_t)V[(curr_op & 0x00F0) >> 4]) {
                        V[15] = 1;
                    }
                    else {
                        V[15] = 0;
                    }
                    V[(curr_op & 0x0F00) >> 8] -= V[(curr_op & 0x00F0) >> 4];
                    break;
                case 0x0006:
                    // Stores the least significant bit of VX in VF and then shifts VX to the right by 1.
                    V[15] = V[(curr_op & 0x0F00) >> 8] & 0b00000001u;
                    V[(curr_op & 0x0F00) >> 8] = (uint8_t)((V[(curr_op & 0x0F00) >> 8] & 0xFF) >> 1);
                    break;
                case 0x0007:
                    // Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there is not.
                    if (V[(curr_op & 0x0F00) >> 8] <= V[(curr_op & 0x00F0) >> 4]) {
                        V[15] = 1;
                    }
                    else {
                        V[15] = 0;
                    }
                    V[(curr_op & 0x0F00) >> 8] = V[(curr_op & 0x00F0) >> 4] - V[(curr_op & 0x0F00) >> 8];
                    break;
                case 0x000E:
                    // Stores the most significant bit of VX in VF and then shifts VX to the left by 1.
                    V[15] = (V[(curr_op & 0x0F00) >> 8] & 0b10000000u) >> 15;
                    V[(curr_op & 0x0F00) >> 8] = (uint8_t)((V[(curr_op & 0x0F00) >> 8] & 0xFF) << 1);
                    break;
                default:
                    cout << "Unknown operation.\n";
                    break;
            }
            break;
        case 0x9000:
            // Skips the next instruction if VX does not equal VY.
            if (V[(curr_op & 0x0F00) >> 8] != V[(curr_op & 0x00F0) >> 4]) {
                PC += 2;
            }
            break;
        case 0xA000:
            // Sets I to the address NNN (from 0xANNN).
            I = curr_op & 0x0FFF;
            break;
        case 0xB000:
            // Jumps to the address NNN plus V0.
            PC = V[0] + (curr_op & 0x0FFF);
            incrementPC = false;
            break;
        case 0xC000:
            // Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
            V[(curr_op & 0x0F00) >> 8] = (rand() % 255) & (curr_op & 0x00FF);
            break;
        case 0xD000:
            // Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. 
            // Each row of 8 pixels is read as bit-coded starting from memory location I; 
            // I value does not change after the execution of this instruction. 
            // As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, 
            // and to 0 if that does not happen
            x = V[(curr_op & 0x0F00) >> 8];
            y = V[(curr_op & 0x00F0) >> 4];
            h = curr_op & 0x000F;
            toggled = false;

            for (int i = y; i < y + h; i++) {
                unsigned char curr_sprite = memory[I + (i - y)];
                for (int j = x; j < x + 8; j++) {
                    if (((curr_sprite >> ((7 + x) - j)) & 1) == 1) {
                        if (display[j%64][i%32] == 1) {
                            display[j%64][i%32] = 0;
                            toggled = true;
                        }
                        else {
                            display[j%64][i%32] = 1;
                        }
                    }
                }
            }

            if (toggled) {
                V[15] = 1;
            }
            else {
                V[15] = 0;
            }

            drawFlag = true;
            break;
        case 0xE000:
            switch (curr_op & 0x000F) {
                case 0x000E:
                    // Skips the next instruction if the key stored in VX is pressed.
                    if (keys[V[(curr_op & 0x0F00) >> 8]] == true) {
                        PC += 2;
                    }
                    break;
                case 0x0001:
                    // Skips the next instruction if the key stored in VX is not pressed.
                    if (keys[V[(curr_op & 0x0F00) >> 8]] != true) {
                        PC += 2;
                    }
                    break;
                default:
                    cout << "Unknown operation.\n";
                    break;
            }
            break;
        case 0xF000:
            switch (curr_op & 0x00FF) {
                case 0x0007:
                    // Sets VX to the value of the delay timer.
                    V[(curr_op & 0x0F00) >> 8] = delay_timer;
                    break;
                case 0x000A:
                    // A key press is awaited, and then stored in VX.
                    for (int i = 0; i < 16; i++) {
                        if (keys[i] == 1) {
                            V[(curr_op & 0x0F00) >> 8] = i;
                            break;
                        }
                        else if (i == 15) {
                            incrementPC = false;
                        }
                    }
                    break;
                case 0x0015:
                    // Sets the delay timer to VX.
                    delay_timer = V[(curr_op & 0x0F00) >> 8];
                    break;
                case 0x0018:
                    // Sets the sound timer to VX.
                    sound_timer = V[(curr_op & 0x0F00) >> 8];
                    break;
                case 0x001E:
                    // Adds VX to I. VF is not affected.
                    I += V[(curr_op & 0x0F00) >> 8];
                    break;
                case 0x0029:
                    // Sets I to the location of the sprite for the character in VX.
                    // Characters 0-F (in hexadecimal) are represented by a 4x5 font.
                    I = font_addr + (V[(curr_op & 0x0F00) >> 8] * 5);
                    break;
                case 0x0033:
                    // Stores the binary-coded decimal representation of VX, 
                    // with the most significant of three digits at the address in I, t
                    // he middle digit at I plus 1, 
                    // and the least significant digit at I plus 2. 
                    // (In other words, take the decimal representation of VX, 
                    // place the hundreds digit in memory at location in I, 
                    // the tens digit at location I+1, and the ones digit at location I+2.)
                    {
                        uint8_t value = V[(curr_op & 0x0F00) >> 8];

                        memory[I + 2] = value % 10;
                        value /= 10;

                        memory[I + 1] = value % 10;
                        value /= 10;

                        memory[I] = value % 10;
                    }
                    break;
                case 0x0055:
                    // Stores from V0 to VX (including VX) in memory, starting at address I. 
                    // The offset from I is increased by 1 for each value written, but I itself is left unmodified.
                    for (int i = 0; i <= (curr_op & 0x0F00) >> 8; i++) {
                        memory[I + i] = V[i];
                    }
                    break;
                case 0x0065:
                    // Fills from V0 to VX (including VX) with values from memory, starting at address I. 
                    // The offset from I is increased by 1 for each value written, but I itself is left unmodified.
                    for (int i = 0; i <= (curr_op & 0x0F00) >> 8; i++) {
                        V[i] = memory[I + i];
                    }
                    break;
                default:
                    cout << "Unknown operation.\n";
                    break;
            }
            break;
        default:
            cout << "Unknown operation.\n";
            break;
    }

    if (delay_timer > 0) {
        delay_timer -= 1;
    }
    
    if (sound_timer > 0) {
        sound_timer -= 1;
        play_sound = true;
    }
    else {
        play_sound = false;
    }

    if (incrementPC) {
        PC += 2;
    }
}


int NES::loadRom(const char* romName) {
    ifstream romFile(romName, ios::in | ios::binary);
    int i = 0;
    unsigned char curr_char = 0;

    vector<char> prog((istreambuf_iterator<char>(romFile)), istreambuf_iterator<char>());

    if (romFile.fail()) {
        return -1;
    }

    copy(prog.begin(), prog.end(), &memory[rom_addr]);

    romFile.close();
    return 0;
}
