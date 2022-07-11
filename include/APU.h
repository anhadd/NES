#ifndef APU_H
#define APU_H

#include <SDL2/SDL.h>
#include <iostream>

#include "GUI.h"

using namespace std;

#define CPU_CLOCK 1789773.0

#define P1_CONTROL          0x4000
#define P1_SWEEP            0x4001
#define P1_TIMER_LOW        0x4002
#define P1_TIMER_HIGH       0x4003

#define P2_CONTROL          0x4004
#define P2_SWEEP            0x4005
#define P2_TIMER_LOW        0x4006
#define P2_TIMER_HIGH       0x4007

#define TR_CONTROL          0x4008
#define TR_UNUSED           0x4009
#define TR_TIMER_LOW        0x400A
#define TR_TIMER_HIGH       0x400B

#define NS_CONTROL          0x400C
#define NS_UNUSED           0x400D
#define NS_PERIOD           0x400E
#define NS_LOAD             0x400F

#define DMC_CONTROL          0x4010
#define DMC_DIRECT           0x4011
#define DMC_ADDRESS          0x4012
#define DMC_LENGTH           0x4013

#define APU_STATUS           0x4015

#define QUARTER_FRAME 3729
#define HALF_FRAME 7457
#define THREE_QUARTER_FRAME 11186
#define FULL_FRAME 14916

#define SAMPLE_SIZE sizeof(int16_t)
#define SAMPLE_TIME_DELTA (1.0 / AUDIO_SAMPLE_RATE)
#define CYCLES_PER_SAMPLE 13125    // For 48000Hz
#define CYCLES_PER_CYCLE 352       // For 48000Hz
// #define CYCLES_PER_SAMPLE 3125     // For 44100Hz
// #define CYCLES_PER_CYCLE 77        // For 44100Hz

#define AUDIO_BUFFER_SIZE 5000


union pulse_control {
    struct {
        uint8_t volume : 4;
        uint8_t constant_volume : 1;
        uint8_t length_halt : 1;
        uint8_t duty : 2;
    };
    uint8_t full;
};

union pulse_sweep {
    struct {
        uint8_t shift_count : 3;
        uint8_t negative : 1;
        uint8_t period : 3;
        uint8_t enabled : 1;
    };
    uint8_t full;
};

// pulse_timers_low is just uint8_t.

union channel_timer_high {
    struct {
        uint8_t timer_high : 3;
        uint8_t length_counter : 5;
    };
    uint8_t full;
};


struct full_pulse {
    union pulse_control ctrl;
    union pulse_sweep sweep;
    uint8_t timer_low;
    union channel_timer_high timer_high;

    uint16_t timer;
    uint16_t reload;
    uint8_t output;

    double duty_partition;
    float sample;

    void reset() {
        ctrl.full = 0x00;
        sweep.full = 0x00;
        timer_low = 0x00;
        timer_high.full = 0x00;

        timer = 0x0000;
        reload = 0x0000;
        duty_partition = 0.00;

        sample = 0.0;
    }
};

union triangle_control {
    struct {
        uint8_t reload : 7;
        uint8_t counter_halt : 1;
    };
    uint8_t full;
};

struct full_triangle {
    union triangle_control ctrl;
    uint8_t timer_low;
    union channel_timer_high timer_high;

    uint16_t timer;
    uint16_t reload;
    uint8_t output;
    int8_t delta;

    float sample;

    void reset() {
        ctrl.full = 0x00;
        timer_low = 0x00;
        timer_high.full = 0x00;

        timer = 0x0000;
        reload = 0x0000;
        output = 15;
        delta = -1;

        sample = 0.0;
    }
};

union channel_status {
    struct {
        uint8_t enable_p1 : 1;
        uint8_t enable_p2 : 1;
        uint8_t enable_triangle : 1;
        uint8_t enable_noise : 1;
        uint8_t enable_dmc : 1;
        uint8_t unused : 3;
    };
    uint8_t full;
};



class APU {
    public:
        GUI* gui;

        double partition_lookup[5];                                         // Lookup table for partitions (used for duty cycles).

        struct full_pulse p1;                                               // Pulse1 channel.
        struct full_pulse p2;                                               // Pulse2 channel.
        struct full_triangle triangle;                                      // Triangle channel.

        union channel_status apu_status;                                    // Status register, enable/disable channels.

        float current_time;                                                 // Time value to sample the square wave.
        uint32_t frame_counter;                                             // APU Frame Counter.
        uint32_t apu_cycles;                                                // Number of cycles executed by the APU.
        uint16_t current_sample_cycle;                                      // Stores the current sample cycle, handles timing sampling.

        int16_t sample;                                                     // Mixed audio sample.
        uint16_t buff_index;                                                // Current index in the audio buffer.
        int16_t audio_buff[AUDIO_BUFFER_SIZE];                              // Audio buffer.

        APU();
        ~APU();

        void passGUI(GUI* nesGUI);                                          // Pass the GUI to the current APU.

        bool executeCycle();                                                // Executes a single APU cycle.
        void reset();                                                       // Reset the APU.

        uint8_t readRegister(uint16_t address);                             // Read from the APU registers (Used by the CPU).
        uint8_t writeRegister(uint16_t address, uint8_t value);             // Write to the APU registers (Used by the CPU).
        void cyclePulse(struct full_pulse pulse);                           // Execute a single cycle for the Pulses.

        float square_wave(struct full_pulse pulse, float offset);           // Generate a square wave from a pulse.
        float triangle_wave(struct full_triangle triangle, float offset);   // Generate a triangle wave from a triangle channel.
};


#endif