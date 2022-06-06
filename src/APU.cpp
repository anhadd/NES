#include "../include/APU.h"



APU::APU() {
    // Constructor
    p1_ctrl.full = 0x00;
    p1_sweep.full = 0x00;
    p1_timer_low = 0x00;
    p1_timer_high.full = 0x00;
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
    uint16_t real_address = 0x2000 + (address & 0x0007);

    switch (real_address) {
        case P1_CONTROL:
            p1_ctrl.full = value;
            break;
        case P1_SWEEP:
            p1_sweep.full = value;
            break;
        case P1_TIMER_LOW:
            p1_timer_low = value;
            break;
        case P1_TIMER_HIGH:
            p1_timer_high.full = value;
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
    }
    return 0;
}

// from: https://stackoverflow.com/questions/62290079/writing-a-wave-generator-with-sdl
double APU::normalize(double phase) {
    double cycles = phase/(2.0*M_PI);
    phase -= trunc(cycles) * 2.0 * M_PI;
    if (phase < 0) {
        phase += 2.0*M_PI;
    }
    return phase;
}

double APU::square(double phase) {
    return (normalize(phase) < M_PI) ? 1.0 : -1.0;
}

double APU::triangle(double phase) {
    phase = normalize(phase);
    if (phase >= M_PI) {
        phase = 2*M_PI - phase;
    }
    return -1.0 + 2.0 * phase / M_PI;
}


// TODO: FIGURE OUT EVERYTHING ABOUT THE API, AND HOW IT GETS THE SOUND
    // ALSO CHECK HOW THE TIMING SHOULD BE, MAYBE ADD A COUNTER OR DELAY / TIMING.

bool APU::executeCycle() {
    // TODO: Tests playing audio, remove later.
    float x = 0;
    // for (int i = 0; i < gui->audio_spec.freq; i++) {
    //     x += .010f;

    //     // SDL_QueueAudio expects a signed 16-bit value
    //     int16_t sample = square(x * 4) * gui->volume;
    //     const int sample_size = sizeof(int16_t) * 1;

    //     SDL_QueueAudio(gui->audio_device, &sample, sample_size);
    // }

    int16_t sample = square(x * 100) * gui->volume;
    const int sample_size = sizeof(int16_t) * 1;

    SDL_QueueAudio(gui->audio_device, &sample, sample_size);
    return 0;
}
