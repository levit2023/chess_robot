#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "stepper.h"
#include <math.h>

// Every time is in seconds

// All 8 stepper steps according to
// https://www.rajguruelectronics.com/Product/1467/28BYJ-48%20-%205V%20Stepper%20Motor.pdf
// Each step is 4 bits
// Clockwise rotation starts at E (step 1) and travels left along stepper_map
const uint32_t stepper_map = 0x673B9DCE;

int step_idx = 0; // indexes through stepper_map

bool clockwise;
uint32_t steps_total;
uint32_t current_step;

double min_step_period = 700 / 1000000.0; // lowest period (s) (top speed)
double max_step_period = 5000 / 1000000.0; // highest period (s) (min speed)
double transition_time = 0.5; // time (s) to transition between min and max period
double alpha; // base for exponential ramp, set by the transition time
double current_step_period;
int ramp_interval; // measures the amount of steps during ramp up, to be able to start ramp down with that many steps left
double steps_start_time;
bool step_done;

void stepper_init_pins() {
    // Currently only supports one stepper motor

    // coils | gpio_set_dir (outputs) | 25-28
    sio_hw->gpio_oe_set |= 0xf << 25;

    // gpio_set_function
    for (int pin=25; pin<=28; pin++) {
        io_bank0_hw->io[pin].ctrl = 5;
        pads_bank0_hw->io[pin] = 0x40;
    }

    // clear pins
    sio_hw->gpio_out &= ~(0xf << 25);
}

void stepper_init_timer() {
    // initialize TIMER0 to fire ALARM0
    hw_set_bits(&timer0_hw->inte, 1u << 0);
    irq_set_exclusive_handler(TIMER0_IRQ_0, stepper_isr);
    irq_set_enabled(TIMER0_IRQ_0, true);
}
// potential free_timer function

void stepper_steps(bool cw, int steps) {
    // set globals
    clockwise = cw;
    steps_total = steps;
    current_step = 0;
    ramp_interval = 0;
    alpha = pow(min_step_period / max_step_period, 1.0/transition_time);
    steps_start_time = ((double)(timer0_hw->timerawl))/1000000.0;
    current_step_period = max_step_period;
    // set timer (s)
    timer0_hw->alarm[0] = (((timer0_hw->timerawl)) + 1000000.0 * current_step_period); 
    step_done = 0;
}

void stepper_isr() {
    // ack interrupt for ALARM0 TIMER0
    hw_clear_bits(&timer0_hw->intr, 1u << 0);

    // step motor by 1 step
    if (clockwise) {
        step_idx = (step_idx + 1) % 8;
    } else {
        step_idx = (step_idx - 1 + 8) % 8;
    }
    sio_hw->gpio_out &= ~(0xf << 25);
    sio_hw->gpio_out |= ((stepper_map >> (4*step_idx)) & 0xf) << 25;

    // determine step_period (s)
    if (current_step > (steps_total - ramp_interval)) {
        // ramp down (increasing period)
        if (current_step == (steps_total - ramp_interval) + 1) {
            steps_start_time = (double)(timer0_hw->timerawl)/1000000.0;
        }
        current_step_period = min_step_period * pow(alpha, -((((double)(timer0_hw->timerawl))/1000000.0 - steps_start_time) - 0.5));
    } else if (current_step_period > min_step_period) {
        // ramp up (decreasing period)
        current_step_period = max_step_period * pow(alpha, (((double)(timer0_hw->timerawl))/1000000.0 - steps_start_time));
        ramp_interval++;
    }

    // set alarm time (s)
    if (current_step < steps_total) {
        timer0_hw->alarm[0] = 1000000.0 * current_step_period + ((timer0_hw->timerawl));
    } else {
        step_done = 1;
    }
    current_step++;
}