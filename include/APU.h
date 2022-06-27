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
#define AUDIO_SAMPLE_RATE 44100.0
#define SAMPLE_TIME_DELTA (1.0 / AUDIO_SAMPLE_RATE)
#define CYCLES_PER_SAMPLE (CPU_CLOCK / 2.0) / AUDIO_SAMPLE_RATE


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

// pulse_timers_low are just uint8_t.

// TODO: Also resets duty and starts envelope.
union pulse_timer_high {
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
    union pulse_timer_high timer_high;

    uint8_t wave_sequence;
    uint16_t timer;
    uint16_t reload;
    uint8_t output;

    double duty_partition;

    void reset() {
        ctrl.full = 0x00;
        sweep.full = 0x00;
        timer_low = 0x00;
        timer_high.full = 0x00;

        wave_sequence = 0x00;
        timer = 0x0000;
        reload = 0x0000;
        duty_partition = 0.00;
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

        uint8_t sequence_lookup[5];
        double partition_lookup[5];

        struct full_pulse p1;
        struct full_pulse p2;

        union channel_status apu_status;

        float current_time;
        uint32_t frame_counter;
        
        float next_sample_cycle;
        uint32_t cycles;

        APU();
        ~APU();

        void passGUI(GUI* nesGUI);

        bool executeCycle();
        void reset();

        uint8_t readRegister(uint16_t address);
        uint8_t writeRegister(uint16_t address, uint8_t value);

        // double normalize(double phase);
        // double square(double phase);
        float square(struct full_pulse pulse, float offset);
        // double triangle(double phase);
        
};


#endif