#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "stepper.h"
#include <math.h>

// Pin definitions
#define RIGHT_COIL1 25
#define RIGHT_COIL2 26
#define RIGHT_COIL3 27
#define RIGHT_COIL4 28
#define LEFT_COIL1 29
#define LEFT_COIL2 30
#define LEFT_COIL3 31
#define LEFT_COIL4 21

// Every time is in seconds

// All 8 stepper steps according to
// https://www.rajguruelectronics.com/Product/1467/28BYJ-48%20-%205V%20Stepper%20Motor.pdf
// Each step is 4 bits (Coil 4 at MSB)
// Clockwise rotation starts at E (step 1) and travels left along stepper_map
const uint32_t stepper_map = 0x673B9DCE;

int right_step_idx = 0; // indexes through stepper_map
int left_step_idx = 0;

bool right_forward;
bool left_forward;
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

uint64_t right_mask = 0;
uint64_t left_mask = 0;

void stepper_init_pins() {
    right_mask |= 1 << RIGHT_COIL1;
    right_mask |= 1 << RIGHT_COIL2;
    right_mask |= 1 << RIGHT_COIL3;
    right_mask |= 1 << RIGHT_COIL4;
    left_mask  |= 1 << LEFT_COIL1;
    left_mask  |= 1 << LEFT_COIL2;
    left_mask  |= 1 << LEFT_COIL3;
    left_mask  |= 1 << LEFT_COIL4;

    gpio_set_dir_out_masked64(right_mask);
    gpio_set_function_masked64(right_mask, GPIO_FUNC_SIO);
    gpio_clr_mask64(right_mask);

    gpio_set_dir_out_masked64(left_mask);
    gpio_set_function_masked64(left_mask, GPIO_FUNC_SIO);
    gpio_clr_mask64(left_mask);
}

void stepper_init_timer() {
    // initialize TIMER0 to fire ALARM0
    hw_set_bits(&timer0_hw->inte, 1u << 0);
    irq_set_exclusive_handler(TIMER0_IRQ_0, stepper_isr);
    irq_set_enabled(TIMER0_IRQ_0, true);
}
// potential free_timer function

void stepper_steps(bool left_fw, bool right_fw, int steps) {
    // set globals
    right_forward = right_fw;
    left_forward = left_fw;
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
    if (right_forward) {
        // right_fw is clockwise
        right_step_idx = (right_step_idx + 1) % 8;
    } else {
        right_step_idx = (right_step_idx - 1 + 8) % 8;
    }
    if (left_forward) {
        // left_fw is counterclockwise
        left_step_idx = (left_step_idx - 1 + 8) % 8;
    } else {
        left_step_idx = (left_step_idx + 1) % 8;
    }
    gpio_clr_mask64(right_mask);
    gpio_clr_mask64(left_mask);
    gpio_set_mask64(((stepper_map >> (4*right_step_idx)) & 0x1) << RIGHT_COIL4);
    gpio_set_mask64(((stepper_map >> (4*right_step_idx)) & 0x2) << (RIGHT_COIL3 - 1));
    gpio_set_mask64(((stepper_map >> (4*right_step_idx)) & 0x4) << (RIGHT_COIL2 - 2));
    gpio_set_mask64(((stepper_map >> (4*right_step_idx)) & 0x8) << (RIGHT_COIL1 - 3));
    gpio_set_mask64(((stepper_map >> (4*left_step_idx)) & 0x1) << LEFT_COIL4);
    gpio_set_mask64(((stepper_map >> (4*left_step_idx)) & 0x2) << (LEFT_COIL3 - 1));
    gpio_set_mask64(((stepper_map >> (4*left_step_idx)) & 0x4) << (LEFT_COIL2 - 2));
    gpio_set_mask64(((stepper_map >> (4*left_step_idx)) & 0x8) << (LEFT_COIL1 - 3));


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