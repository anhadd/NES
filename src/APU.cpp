#include "../include/APU.h"



APU::APU() {
    // Constructor
    p1.reset();
    p2.reset();

    sequence_lookup[0] = 0b01000000;
    sequence_lookup[1] = 0b01100000;
    sequence_lookup[2] = 0b01111000;
    sequence_lookup[3] = 0b10011111;

    partition_lookup[0] = 0.125;
    partition_lookup[1] = 0.250;
    partition_lookup[2] = 0.500;
    partition_lookup[3] = 0.750;

    apu_status.full = 0x00;
    current_time = 0.0;
    frame_counter = 0x00000000;

    next_sample_cycle = 0x0000;
}

APU::~APU() {
    // Destructor
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
            p1.wave_sequence = sequence_lookup[p1.ctrl.duty];
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
        case P2_SWEEP:
        case P2_TIMER_LOW:
        case P2_TIMER_HIGH:

        case TR_CONTROL:
        case TR_UNUSED:
        case TR_TIMER_LOW:
        case TR_TIMER_HIGH:

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
            break;
    }
    return 0;
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
    float frequency = CPU_CLOCK / (16.0 * (double)(p1.reload + 1));
    // float frequency = 440.0f;
    float phase_offset = pulse.duty_partition * 2.0f * M_PI;

    for (float counter = 1; counter < 8; counter++) {
        temp = counter * frequency * 2.0 * M_PI * offset;

        sin1 += approxsin(temp) / counter;
        sin2 += approxsin(temp - phase_offset * counter) / counter;
    }

    return (2.0 / M_PI) * (sin1 - sin2); 
}


// TODO: FIGURE OUT EVERYTHING ABOUT THE APU, AND HOW IT GETS THE SOUND
    // ALSO CHECK HOW THE TIMING SHOULD BE, MAYBE ADD A COUNTER OR DELAY / TIMING.

// CHECK THE FRAME COUNTER STUFF TO GET THE TIMING RIGHT! RN IT ADDS TOO QUICKLY TO THE BUFFER.

bool APU::executeCycle() {
    if (frame_counter == QUARTER_FRAME || frame_counter == THREE_QUARTER_FRAME) {
        // Quarter frames.
    }
    else if (frame_counter == HALF_FRAME || frame_counter == FULL_FRAME) {
        // Quarter and Half frames.
        if (p1.timer_high.length_counter > 0) { // && !p1.ctrl.length_halt
            p1.timer_high.length_counter -= 1;
        }
        // else if (p1.timer_high.length_counter == 0) {
        //     apu_status.enable_p1 = 0;
        // }

        if (frame_counter == FULL_FRAME) {
            frame_counter = 0;
        }
    }

    if (cycles >= next_sample_cycle) {
    // if (SDL_GetQueuedAudioSize(gui->audio_device) <= 3675) {
        next_sample_cycle += CYCLES_PER_SAMPLE * 24.0;
        current_time += SAMPLE_TIME_DELTA;
        
        if (apu_status.enable_p1) {
            p1.timer -= 1;
            if (p1.timer == 0xFFFF) {
                p1.timer = p1.reload + 1;
                p1.output = (p1.output & 0x01 << 7) | (p1.output & 0xFE >> 1);
            }

            sample = square(p1, current_time) * gui->volume;
            // int16_t sample = sin(current_time * 440.0f * 2.0f * M_PI) * gui->volume;
            
            // SDL_QueueAudio(gui->audio_device, &sample, SAMPLE_SIZE);
        }
    }

    // queue_size = SDL_GetQueuedAudioSize(gui->audio_device);
    // if (queue_size < LOWER_QUEUE) next_sample_cycle += 3;
    // if (queue_size > UPPER_QUEUE) next_sample_cycle -= 3;
    // if (queue_size > UPPER_QUEUE) SDL_ClearQueuedAudio(gui->audio_device);

    frame_counter += 1;
    cycles += 24;
    return 0;
}
