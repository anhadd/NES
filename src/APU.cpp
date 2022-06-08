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

    current_sample_cycle = 0x00;
    cycles_per_sample = 0x00;
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
            p1.reload = ((p1.timer_high.full & 0x07) << 8) | (p1.reload & 0x00FF);
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

// TODO: check this for testing, from olc.
float APU::square(struct full_pulse pulse, float offset) {
    float temp = 0.0;
    float sin1 = 0.0;
    float sin2 = 0.0;
    // float frequency = CPU_CLOCK / (16.0 * (double)(p1.reload + 1));
    float frequency = CPU_CLOCK / (16 * (offset + 1));
    // float frequency = 440.0f;
    // float frequency = p1.timer;
    float phase_offset = pulse.duty_partition;

    // fprintf(stderr, "FREQUENCY: %f\n", frequency);

    for (float counter = 1; counter < 20; counter++) {
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
    if (current_sample_cycle == cycles_per_sample) {
        current_sample_cycle = 0x00;
        // current_time += (2 / CPU_CLOCK);
        current_time += SAMPLE_TIME_DELTA; // TODO: /60 for the frames?
        // current_time += 1 / (CPU_CLOCK / 2);

        if (frame_counter == QUARTER_FRAME || frame_counter == THREE_QUARTER_FRAME) {
            // Quarter frame.
        }
        else if (frame_counter == HALF_FRAME) {
            // Quarter and Half frame.
        }
        else if (frame_counter == FULL_FRAME) {
            // Quarter and Half frame.
            frame_counter = 0;
        }

        p1.executeCycle(apu_status.enable_p1);

        if (apu_status.enable_p1) {
            // p1.wave_sequence = ((p1.wave_sequence & 0x01) << 7) || (p1.wave_sequence >> 1);

            int16_t sample = square(p1, current_time) * gui->volume; // Was current_time * 4
            
            // int16_t sample = (p1.wave_sequence << 8) | p1.wave_sequence;
            // int16_t sample = sin(current_time * 440.0f * 2.0f * M_PI) * gui->volume;
            const int sample_size = sizeof(int16_t);
            
            SDL_QueueAudio(gui->audio_device, &sample, sample_size);
        }

        frame_counter += 1;
    }
    current_sample_cycle += 1;
    return 0;
}
