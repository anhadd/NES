#include "../include/APU.h"



APU::APU() {
    // Constructor
    p1.reset();

    partition_lookup = {
        0.125, 0.250, 0.500, 0.750
    };

    apu_status.full = 0x00;
    frame_counter = 0x00000000;

    current_time = 0.0;
    current_sample_cycle = 0x00;
}

APU::~APU() {
    // Destructor
}

void APU::reset() {
    apu_status.full = 0x00;
    frame_counter = 0x00000000;

    p1.reset();
}

void APU::passGUI(GUI* nesGUI) {
    gui = nesGUI;
}

uint8_t APU::readRegister(uint16_t address) {
    uint16_t real_address = 0x4000 + (address & 0x0017);
    uint8_t value = 0x00;

    if (real_address == APU_STATUS) {
        value |= (p1.length_counter > 0) ? 0x1 : 0;
    }
    return value;
}

uint8_t APU::writeRegister(uint16_t address, uint8_t value) {
    uint16_t real_address = 0x4000 + (address & 0x1F);

    switch (real_address) {
        case P1_CONTROL:
            p1.ctrl.full = value;
            p1.duty_partition = partition_lookup[p1.ctrl.duty];
            break;
        case P1_SWEEP:
            break;
        case P1_TIMER_LOW:
            p1.timer_low = value;
            p1.reload = (p1.reload & 0xFF00) | p1.timer_low;
            if (debug_log) {
                fprintf(stderr, "WRITE LOW:  %u\n", p1.reload);
                fprintf(stderr, "VALUE:      %u\n\n", value);
            }
            break;
        case P1_TIMER_HIGH:
            p1.timer_high.full = value;
            p1.reload = ((p1.timer_high.timer_high) << 8) | (p1.reload & 0x00FF);
            if (debug_log) {
                fprintf(stderr, "WRITE HIGH: %u\n", p1.reload);
                fprintf(stderr, "VALUE:      %u\n\n", value);
            }
            break;

        case APU_STATUS:
            // Enable/disable Pulse1.
            apu_status.enable_p1 = value & 0x01;
            if (!apu_status.enable_p1) {
                p1.length_counter = 0;
            }
            break;
    }
    return 0;
}

void APU::generateSample(struct full_pulse &pulse) {
    // Volume source is determined by the constant_volume flag.
    if (pulse.ctrl.constant_volume) {
        pulse.sample = (pulse.ctrl.volume / 16.0) * square_wave(pulse, current_time);
    }
    else {
        pulse.sample = (pulse.env.decay_level / 16.0) * square_wave(pulse, current_time);
    }
}

// Used for approximating sin, since the normal sin function is slow.
// from: One Lone Coder.
float approxsin(float offset) {
    float j = offset * 0.15915;
    j = j - (int)j;
    return 20.785 * j * (j - 0.5) * (j - 1.0f);
}

float APU::square_wave(struct full_pulse pulse, float offset) {
    float temp = 0.0;
    float sin1 = 0.0;
    float sin2 = 0.0;
    // Calculate the frequency and offset for duty cycles.
    float frequency = CPU_CLOCK / (16.0 * (float)(pulse.reload + 1));
    float phase_offset = pulse.duty_partition * 2.0 * M_PI;
    // Combine multiple sine waves to get close to a square wave.
    for (float counter = 1; counter < 4; counter++) {
        temp = counter * frequency * 2.0 * M_PI * offset;

        sin1 += -approxsin(temp) / counter;
        sin2 += -approxsin(temp - phase_offset * counter) / counter;
    }
    // Set the output to 1 or -1 depending on the sine wave value.
    if (sin1 - sin2 >= 0) {
        return 1.0;
    }
    else {
        return -1.0;
    }
}

bool APU::executeCycle() {
    // Every 2 CPU/APU cycles.
    if (apu_cycles % 2 == 0) {
        frame_counter += 1;
        // Quarter frame.
        if (frame_counter == QUARTER_FRAME || frame_counter == THREE_QUARTER_FRAME) {
            // Clock the envelopes.
            // p1.clockEnvelope();
            // p2.clockEnvelope();
        }
        // Half frame.
        else if (frame_counter == HALF_FRAME || frame_counter == FULL_FRAME) {
            // Decrement length counters if necessary.
            // if (!p1.ctrl.length_halt && p1.length_counter > 0) {
            //     p1.length_counter -= 1;
            // }
            // Sweep units.
            // ...
            if (frame_counter == FULL_FRAME) {
                frame_counter = 0;
            }
        }

        p1.sample = 0.0;
        // Cycle and get the sample for Pulse1 if it is enabled.
        p1.cycle();
        if (apu_status.enable_p1) { //  && p1.length_counter > 0 && p1.timer >= 8
            generateSample(p1);
        }
    }

    // Every certain amount of cycles store a sample for audio output.
    if (current_sample_cycle >= CYCLES_PER_SAMPLE) {
        // Increment the time for the current sample.
        current_time += SAMPLE_TIME_DELTA;
        current_sample_cycle -= CYCLES_PER_SAMPLE;

        sample = p1.sample * 200;

        // Add sample to audio buffer.
        if (debug_log) {
            fprintf(stderr, "FREQ: %f\n", CPU_CLOCK / (16.0 * (double)(p1.reload + 1)));
            fprintf(stderr, "RELOAD: %u\n", p1.reload);
        }
        SDL_QueueAudio(gui->audio_device, &sample, SAMPLE_SIZE);
    }

    apu_cycles += 1;
    current_sample_cycle += CYCLES_PER_CYCLE;
    return 0;
}
