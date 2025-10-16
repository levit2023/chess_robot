#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "movement.h"
#include "utils.h"
#include <math.h>

// pin definitions

// motor A (left) coils
#define LEFT_COIL1 4
#define LEFT_COIL2 3
#define LEFT_COIL3 2
#define LEFT_COIL4 0

// motor B (right) coils
#define RIGHT_COIL1 11
#define RIGHT_COIL2 7
#define RIGHT_COIL3 6
#define RIGHT_COIL4 5

// all 8 stepper steps according to
// https://www.rajguruelectronics.com/Product/1467/28BYJ-48%20-%205V%20Stepper%20Motor.pdf
// clockwise rotation starts at 0xE (step 1) and ends at 0x6 (step 8)
static const uint8_t stepper_sequence[8] = {0xE, 0xC, 0xD, 0x9, 0xB, 0x3, 0x7, 0x6};

// each step is 4 bits (Coil 4 at MSB)
static const uint8_t right_coil_pins[4] = {RIGHT_COIL4, RIGHT_COIL3, RIGHT_COIL2, RIGHT_COIL1};
static const uint8_t left_coil_pins[4] = {LEFT_COIL4, LEFT_COIL3, LEFT_COIL2, LEFT_COIL1};

int left_step_idx = 0; // indexes through stepper_map
int right_step_idx = 0; // indexes through stepper_map

direction_t direction; // movement direction

uint32_t total_steps; // total steps in this motion
uint32_t step_count; // steps taken so far this motion
uint32_t ramp_steps; // measures the amount of steps during ramp up, to be able to start ramp down with that many steps left

double current_speed = 0; // current speed (steps/s)
double min_speed = 200; // lowest speed (steps/s)
double max_speed = 1500; // highest speed (steps/s)
double decel_speed_initial = 0; // initial speed (steps/s) for decel phase
double accel = 1000; // acceleration (steps/s^2)

double phase_start_time; // initial time (s) for accel/decel calculation

bool stepper_idle; // low whenever movement is in progress

stepper_state_t state; // accel, cruise, decel

void stepper_init_pins() {
    for (int i = 0; i < 4; i++) {
        gpio_set_dir(right_coil_pins[i], true); // output
        gpio_set_function(right_coil_pins[i], GPIO_FUNC_SIO);
        gpio_put(right_coil_pins[i], false); // clear

        gpio_set_dir(left_coil_pins[i], true); // output
        gpio_set_function(left_coil_pins[i], GPIO_FUNC_SIO);
        gpio_put(left_coil_pins[i], false); // clear
    }
}

void stepper_init_timer() {
    // initialize TIMER0 to fire ALARM0
    hw_set_bits(&timer0_hw->inte, 1u << 0);
    irq_set_exclusive_handler(TIMER0_IRQ_0, stepper_isr);
    irq_set_enabled(TIMER0_IRQ_0, true);
}

void stepper_steps(direction_t dir, int steps) {
    // set globals
    direction = dir;
    total_steps = steps;
    step_count = 0;
    ramp_steps = 0;
    
    phase_start_time = us_to_s(timer0_hw->timerawl);
    stepper_idle = 0;
    state = STEPPER_ACCEL;
    
    // set alarm time
    timer0_hw->alarm[0] = s_to_us(1.0/max_speed) + timer0_hw->timerawl; 
}

void stepper_isr() {
    // ack interrupt for ALARM0 TIMER0
    hw_clear_bits(&timer0_hw->intr, 1u << 0);

    // step motor by 1 step
    if ((direction == FORWARD) || (direction == TURN_LEFT)) {
        // right motor forward == clockwise
        right_step_idx = (right_step_idx + 1) % 8;
    } else {
        right_step_idx = (right_step_idx - 1 + 8) % 8;
    }
    if ((direction == FORWARD) || (direction == TURN_RIGHT)) {
        // left motor forward == counterclockwise
        left_step_idx = (left_step_idx - 1 + 8) % 8;
    } else {
        left_step_idx = (left_step_idx + 1) % 8;
    }
    
    // set pins according to stepper_sequence
    for (int i = 0; i < 4; i++) {
        gpio_put(right_coil_pins[i], (stepper_sequence[right_step_idx] >> i) & 0x1);
        gpio_put(left_coil_pins[i], (stepper_sequence[left_step_idx] >> i) & 0x1);
    }
    step_count++; //moved to here

    // state machine for accel, cruise, decel
    switch (state) {
        case STEPPER_ACCEL:
            current_speed = min_speed + accel * (us_to_s(timer0_hw->timerawl) - phase_start_time);
            if (current_speed >= max_speed) {
                current_speed = max_speed;
                state = STEPPER_CRUISE;
            } else if (step_count >= (total_steps / 2.0)) {
                // skip to decel if not enough ramp steps
                decel_speed_initial = current_speed;
                state = STEPPER_DECEL;
                phase_start_time = us_to_s(timer0_hw->timerawl);
            }
            ramp_steps++; // count ramp_steps during accel
            break;
        case STEPPER_CRUISE:
            current_speed = max_speed;
            decel_speed_initial = current_speed;
            if (step_count >= (total_steps - ramp_steps)) {
                state = STEPPER_DECEL;
                phase_start_time = us_to_s(timer0_hw->timerawl);
            }
            break;
        case STEPPER_DECEL:
            current_speed = decel_speed_initial - accel * (us_to_s(timer0_hw->timerawl) - phase_start_time);
            if (current_speed < min_speed) current_speed = min_speed;
    }

    // set alarm time
    if (step_count < total_steps) {
        timer0_hw->alarm[0] = s_to_us(1.0/current_speed) + timer0_hw->timerawl;
    } else {
        stepper_idle = 1;
        // set pins according to stepper_sequence
        for (int i = 0; i < 4; i++) {
            gpio_put(right_coil_pins[i], (0x0 >> i) & 0x1);
            gpio_put(left_coil_pins[i], (0x0 >> i) & 0x1);
        }
    }
}