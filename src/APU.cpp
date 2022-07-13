#include "../include/APU.h"



APU::APU() {
    // Constructor
    p1.reset();
    p2.reset();

    partition_lookup = {
        0.125, 0.250, 0.500, 0.750
    };

    length_lookup = {
         10, 254,  20,   2,  40,   4,  80,   6, 160,   8,  60,  10,  14,  12,  26,  14,
         12,  16,  24,  18,  48,  20,  96,  22, 192,  24,  72,  26,  16,  28,  32,  30
    };

    apu_status.full = 0x00;
    current_time = 0.0;
    frame_counter = 0x0000000F;

    current_sample_cycle = 0x00;

    buff_index = 0;
    fill(begin(audio_buff), end(audio_buff), 0);
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

void APU::passGUI(GUI* nesGUI) {
    gui = nesGUI;
}

uint8_t APU::readRegister(uint16_t address) {
    uint16_t real_address = 0x4000 + (address & 0x1F);
    uint8_t value = 0x00;

    if (real_address == APU_STATUS) {
        value |= (p1.length_counter > 0) ? 0x1 : 0;
        value |= (p2.length_counter > 0) ? 0x2 : 0;
        value |= (triangle.length_counter > 0) ? 0x4 : 0;
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
            p1.sweep.full = value;
            break;
        case P1_TIMER_LOW:
            p1.timer_low = value;
            p1.reload = (p1.reload & 0xFF00) | p1.timer_low;
            break;
        case P1_TIMER_HIGH:
            p1.timer_high.full = value;
            if (apu_status.enable_p1) {
                p1.length_counter = length_lookup[p1.timer_high.length_counter_load];
            }
            p1.reload = ((p1.timer_high.timer_high) << 8) | (p1.reload & 0x00FF);
            p1.timer = p1.reload;
            p1.env.start_flag = true;
            break;

        case P2_CONTROL:
            p2.ctrl.full = value;
            p2.duty_partition = partition_lookup[p2.ctrl.duty];
            break;
        case P2_SWEEP:
            p2.sweep.full = value;
            break;
        case P2_TIMER_LOW:
            p2.timer_low = value;
            p2.reload = (p2.reload & 0xFF00) | p2.timer_low;
            break;
        case P2_TIMER_HIGH:
            p2.timer_high.full = value;
            if (apu_status.enable_p2) {
                p2.length_counter = length_lookup[p2.timer_high.length_counter_load];
            }
            p2.reload = ((p2.timer_high.timer_high) << 8) | (p2.reload & 0x00FF);
            p2.timer = p2.reload;
            p2.env.start_flag = true;
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
            if (apu_status.enable_triangle) {
                triangle.length_counter = length_lookup[triangle.timer_high.length_counter_load];
            }
            triangle.reload = ((triangle.timer_high.timer_high) << 8) | (triangle.reload & 0x00FF);
            triangle.timer = triangle.reload;
            break;

        // case NS_CONTROL:
        // case NS_UNUSED:
        // case NS_PERIOD:
        // case NS_LOAD:

        // case DMC_CONTROL:
        // case DMC_DIRECT:
        // case DMC_ADDRESS:
        // case DMC_LENGTH:
        
        case APU_STATUS:
            // Enable/disable Pulse1.
            apu_status.enable_p1 = value & 0x01;
            if (!apu_status.enable_p1) {
                p1.length_counter = 0;
            }
            // Enable/disable Pulse2.
            apu_status.enable_p2 = (value & 0x02) != 0;
            if (!apu_status.enable_p2) {
                p2.length_counter = 0;
            }
            // Enable/disable Triangle.
            apu_status.enable_triangle = (value & 0x04) != 0;
            if (!apu_status.enable_triangle) {
                triangle.length_counter = 0;
            }
            break;
    }
    return 0;
}

void APU::generateSample(struct full_pulse &pulse) {
    pulse.sample = 0.0;
    // Volume source is determined by the constant_volume flag.
    if (pulse.ctrl.constant_volume) {
        pulse.sample = (pulse.ctrl.volume / 16.0) * square_wave(pulse, current_time);
    }
    else {
        pulse.sample = (pulse.env.decay_level / 16.0) * square_wave(pulse, current_time);
    }
}

void APU::outputMixedSample() {
    // Increment the time for the current sample.
    current_time += SAMPLE_TIME_DELTA;
    current_sample_cycle -= CYCLES_PER_SAMPLE;
    // Mix the separate channels.
    sample = ((0.00752 * (p1.sample + p2.sample))
                + (0.00851 * triangle.sample + 0.00494 * /*noise*/0 + 0.00335 * /*DMC*/0)) * gui->volume;

    // For debugging: outputs a single continuous tone.
    // sample = sin(current_time * 440.0f * 2.0f * M_PI) * 2000;
    
    // Add sample to audio buffer.
    audio_buff[buff_index] = sample;
    buff_index++;
    // If the audio buffer is full send it to output.
    if (buff_index == AUDIO_BUFFER_SIZE) {
        buff_index = 0;
        SDL_QueueAudio(gui->audio_device, &audio_buff, SAMPLE_SIZE * AUDIO_BUFFER_SIZE);
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
    for (float counter = 1; counter < 3; counter++) {
        temp = counter * frequency * 2.0 * M_PI * offset;

        sin1 += approxsin(temp) / counter;
        sin2 += approxsin(temp - phase_offset * counter) / counter;
    }
    // Set the output to 1 or -1 depending on the sine wave value.
    if (sin1 - sin2 >= 0) {
        return 1.0;
    }
    else {
        return -1.0;
    }
}

float APU::triangle_wave(struct full_triangle triangle, float offset) {
    float n = 0.0;
    float temp = 0.0;
    float sin = 0.0;
    // Calculate the frequency.
    float frequency = CPU_CLOCK / (32.0 * (float)(triangle.reload + 1));
    // Combine multiple sine waves to get close to a triangle wave.
    for (float counter = 1; counter < 3; counter++) {
        n = (2 * counter) + 1;
        temp = 2.0 * M_PI * frequency * n * offset;
        sin += pow(-1, counter) * pow(n, -2) * approxsin(temp);
    }
    return (8 / pow(M_PI, 2)) * sin;
}

bool APU::executeCycle() {
    // Clock triangle channel every CPU cycle.
    triangle.timer -= 1;
    if (triangle.timer == 0xFFFF) {
        triangle.timer = triangle.reload + 1;
    }

    // Every 2 CPU/APU cycles.
    if (apu_cycles % 2 == 0) {
        frame_counter += 1;
        // Quarter frame.
        if (frame_counter == QUARTER_FRAME || frame_counter == THREE_QUARTER_FRAME) {
            // Clock the envelopes.
            p1.clockEnvelope();
            p2.clockEnvelope();
        }
        // Half frame.
        else if (frame_counter == HALF_FRAME || frame_counter == FULL_FRAME) {
            // Decrement length counters if necessary.
            // if (!p1.ctrl.length_halt && p1.length_counter > 0) {
            //     p1.length_counter -= 1;
            // }
            // if (!p2.ctrl.length_halt && p2.length_counter > 0) {
            //     p2.length_counter -= 1;
            // }
            // if (!triangle.ctrl.length_halt && triangle.length_counter > 0) {
            //     triangle.length_counter -= 1;
            // }
            // Sweep units.
            // ...
            if (frame_counter == FULL_FRAME) {
                frame_counter = 0;
            }
        }

        // Cycle and get the sample for Pulse1 if it is enabled.
        p1.cycle();
        if (apu_status.enable_p1 && p1.length_counter > 0 && p1.timer >= 8) {
            generateSample(p1);
        }
        // Cycle and get the sample for Pulse2 if it is enabled.
        p2.cycle();
        if (apu_status.enable_p2 && p2.length_counter > 0 && p2.timer >= 8) {
            generateSample(p2);
        }
        // Cycle and get the sample for Triangle if it is enabled.
        triangle.sample = 0.0;
        if (apu_status.enable_triangle && triangle.length_counter > 0 && triangle.timer >= 8) {
            triangle.sample = triangle_wave(triangle, current_time);
        }
    }

    // Every certain amount of cycles store a sample for audio output.
    if (current_sample_cycle >= CYCLES_PER_SAMPLE) {
        outputMixedSample();
    }

    apu_cycles += 1;
    current_sample_cycle += CYCLES_PER_CYCLE;
    return 0;
}
