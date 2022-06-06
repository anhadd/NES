#ifndef APU_H
#define APU_H

#include <SDL2/SDL.h>
#include <iostream>

#include "GUI.h"

using namespace std;

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


union pulse_control {
    struct {
        uint8_t volume : 4;
        uint8_t constant_volume : 1;
        uint8_t loop : 1;
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

// Also resets duty and starts envelope
union pulse_timer_high {
    struct {
        uint8_t timer_high : 3;
        uint8_t length_counter : 5;
    };
    uint8_t full;
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

        union pulse_control p1_ctrl;
        union pulse_sweep p1_sweep;
        uint8_t p1_timer_low;
        union pulse_timer_high p1_timer_high;

        union channel_status apu_status;

        APU();
        ~APU();

        void passGUI(GUI* nesGUI);

        bool executeCycle();
        void reset();

        uint8_t readRegister(uint16_t address);
        uint8_t writeRegister(uint16_t address, uint8_t value);

        double normalize(double phase);
        double square(double phase);
        double triangle(double phase);
};


#endif
