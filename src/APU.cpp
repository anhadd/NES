#include "../include/APU.h"



APU::APU() {
    // Constructor
    p1.reset();
    p2.reset();

    // sequence_lookup[0] = 0b01000000;
    // sequence_lookup[1] = 0b01100000;
    // sequence_lookup[2] = 0b01111000;
    // sequence_lookup[3] = 0b10011111;

    partition_lookup[0] = 0.125;
    partition_lookup[1] = 0.250;
    partition_lookup[2] = 0.500;
    partition_lookup[3] = 0.750;

    apu_status.full = 0x00;
    current_time = 0.0;
    frame_counter = 0x0000000F;

    current_sample_cycle = 0x00;
}

APU::~APU() {
    // Destructor
    
}

void APU::reset() {
    apu_status.full = 0x00;
    frame_counter = 0x0000000F;

    p1.reset();
    p2.reset();
    triangle.reset();
}

// Pass the GUI to the current APU.
void APU::passGUI(GUI* nesGUI) {
    gui = nesGUI;
}

// Read from the APU registers (Used by the CPU).
uint8_t APU::readRegister(uint16_t address) {
    uint16_t real_address = 0x4000 + (address & 0x0017);

    switch (real_address) {
        
    }
    return 0x00;
}


// Write to the APU registers (Used by the CPU).
uint8_t APU::writeRegister(uint16_t address, uint8_t value) {
    uint16_t real_address = 0x4000 + (address & 0x0017);

    switch (real_address) {
        case P1_CONTROL:
            p1.ctrl.full = value;
            // p1.wave_sequence = sequence_lookup[p1.ctrl.duty];
            p1.duty_partition = partition_lookup[p1.ctrl.duty];
            break;
        case P1_SWEEP:
            p1.sweep.full = value;
            break;
        case P1_TIMER_LOW:
            p1.timer_low = value;
            p1.reload = (p1.reload & 0xFF00) | p1.timer_low;
            break;
        case P1_TIMER_HIGH:
            p1.timer_high.full = value;
            p1.reload = ((p1.timer_high.timer_high) << 8) | (p1.reload & 0x00FF);
            p1.timer = p1.reload;
            break;

        case P2_CONTROL:
            p2.ctrl.full = value;
            // p2.wave_sequence = sequence_lookup[p2.ctrl.duty];
            p2.duty_partition = partition_lookup[p2.ctrl.duty];
            break;
        case P2_SWEEP:
            p2.timer_low = value;
            p2.reload = (p2.reload & 0xFF00) | p2.timer_low;
            break;
        case P2_TIMER_LOW:
            p2.timer_low = value;
            p2.reload = (p2.reload & 0xFF00) | p2.timer_low;
            break;
        case P2_TIMER_HIGH:
            p2.timer_high.full = value;
            p2.reload = ((p2.timer_high.full & 0x07) << 8) | (p2.reload & 0x00FF);
            p2.timer = p2.reload;
            break;

        case TR_CONTROL:
            triangle.ctrl.full = value;
            break;
        case TR_UNUSED:
            // Unused.
            break;
        case TR_TIMER_LOW:
            triangle.timer_low = value;
            triangle.reload = (triangle.reload & 0xFF00) | triangle.timer_low;
            break;
        case TR_TIMER_HIGH:
            triangle.timer_high.full = value;
            triangle.reload = ((triangle.timer_high.timer_high) << 8) | (triangle.reload & 0x00FF);
            triangle.timer = triangle.reload;
            break;

        case NS_CONTROL:
        case NS_UNUSED:
        case NS_PERIOD:
        case NS_LOAD:

        case DMC_CONTROL:
        case DMC_DIRECT:
        case DMC_ADDRESS:
        case DMC_LENGTH:
            break;
        case APU_STATUS:
            apu_status.enable_p1 = value & 0x01;
            if (!apu_status.enable_p1) {
                p1.timer_high.length_counter = 0;
            }
            apu_status.enable_p2 = (value & 0x02) != 0;
            if (!apu_status.enable_p2) {
                p2.timer_high.length_counter = 0;
            }
            apu_status.enable_triangle = (value & 0x04) != 0;
            if (!apu_status.enable_triangle) {
                triangle.timer_high.length_counter = 0;
            }
            break;
    }
    return 0;
}

void APU::cyclePulse(struct full_pulse pulse) {
    pulse.timer -= 1;
    if (pulse.timer == 0xFFFF) {
        pulse.timer = pulse.reload + 1;
        pulse.output = (pulse.output & 0x01 << 7) | (pulse.output & 0xFE >> 1);
    }
}

// Used for approximating sin, since the sin function is very slow.
// from: OLC.
float approxsin(float offset) {
    float j = offset * 0.15915;
    j = j - (int)j;
    return 20.785 * j * (j - 0.5) * (j - 1.0f);
}

float APU::square(struct full_pulse pulse, float offset) {
    float temp = 0.0;
    float sin1 = 0.0;
    float sin2 = 0.0;
    float frequency = CPU_CLOCK / (16.0 * (double)(pulse.reload + 1));
    float phase_offset = pulse.duty_partition * 2.0f * M_PI;

    for (float counter = 1; counter < 2; counter++) {
        temp = counter * frequency * 2.0 * M_PI * offset;

        sin1 += approxsin(temp) / counter;
        sin2 += approxsin(temp - phase_offset * counter) / counter;
    }

    return (2.0 / M_PI) * (sin1 - sin2);
}

float APU::triangle_wave(struct full_triangle triangle, float offset) {
    float n = 0.0;
    float temp = 0.0;
    float sin1 = 0.0;
    float frequency = CPU_CLOCK / (32.0 * (double)(triangle.reload + 1));

    for (float counter = 1; counter < 2; counter++) {
        n = (2 * counter) + 1;
        temp = 2.0 * M_PI * frequency * n * offset;
        sin1 += pow(-1, counter) * pow(n, -2) * approxsin(temp) / counter;
    }

    return (8 / pow(M_PI, 2)) * sin1; 
}


// TODO: FIGURE OUT EVERYTHING ABOUT THE APU, AND HOW IT GETS THE SOUND
    // ALSO CHECK HOW THE TIMING SHOULD BE, MAYBE ADD A COUNTER OR DELAY / TIMING.

// CHECK THE FRAME COUNTER STUFF TO GET THE TIMING RIGHT! RN IT ADDS TOO QUICKLY TO THE BUFFER.

// TODO: TEST ON A FASTER COMPUTER!!!!

bool APU::executeCycle() {
    if (apu_cycles % 2 == 0) {
        if (frame_counter == QUARTER_FRAME || frame_counter == THREE_QUARTER_FRAME) {
            // Quarter frame.
        }
        else if (frame_counter == HALF_FRAME || frame_counter == FULL_FRAME) {
            //  Half frame.
            if (frame_counter == FULL_FRAME) {
                frame_counter = 0;
            }
        }

        sample = 0x0000;
        if (apu_status.enable_p1) {
            cyclePulse(p1);
            if (p1.ctrl.constant_volume) {
                sample += (p1.ctrl.volume != 0) * square(p1, current_time) * gui->volume;
            }
        }
        if (apu_status.enable_p2) {
            cyclePulse(p2);
            if (p2.ctrl.constant_volume) {
                sample += (p2.ctrl.volume != 0) * square(p2, current_time) * gui->volume;
            }
        }
        if (apu_status.enable_triangle) {
            triangle.timer -= 1;
            if (triangle.timer == 0xFFFF) {
                triangle.timer = triangle.reload + 1;
                triangle.output += triangle.delta;
                if (triangle.output == 15) {
                    triangle.delta = -1;
                }
                else if (triangle.output == 0) {
                    triangle.delta = 1;
                }
            }

            // sample += triangle.output * gui->volume;
            sample += triangle_wave(triangle, current_time) * gui->volume;
        }

        // sample += sin(current_time * 440.0f * 2.0f * M_PI) * gui->volume;
        frame_counter += 1;
    }

    if (current_sample_cycle >= CYCLES_PER_SAMPLE) {
        current_time += SAMPLE_TIME_DELTA;
        current_sample_cycle -= CYCLES_PER_SAMPLE;
        SDL_QueueAudio(gui->audio_device, &sample, SAMPLE_SIZE);

        // if (SDL_GetQueuedAudioSize(gui->audio_device) > 8192) {
        //     SDL_DequeueAudio(gui->audio_device, NULL, 4096);
        // }
    }

    apu_cycles += 1;
    current_sample_cycle += 352;
    return 0;
}
