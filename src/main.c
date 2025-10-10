#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "stepper.h"
#include <math.h>

int main()
{
    // Configures our microcontroller to 
    // communicate over UART through the TX/RX pins
    stdio_init_all();

    // test code
    stepper_init_pins();
    stepper_init_timer();

    // pending: forward, left_turn, right_turn movement functions
    stepper_steps(true, true, 2000); // 4096 is one revolution
    while (!step_done) {
        tight_loop_contents();
    }

    stepper_steps(true, false, 2000);
    while (!step_done) {
        tight_loop_contents();
    }

    stepper_steps(true, true, 2000);
    while (!step_done) {
        tight_loop_contents();
    }

    stepper_steps(false, true, 2000);
    while (!step_done) {
        tight_loop_contents();
    }

    stepper_steps(true, true, 2000);
    while (!step_done) {
        tight_loop_contents();
    }

    stepper_steps(false, true, 2000);
    while (!step_done) {
        tight_loop_contents();
    }

    stepper_steps(true, true, 2000);
    while (!step_done) {
        tight_loop_contents();
    }

    stepper_steps(true, false, 2000);
    while (!step_done) {
        tight_loop_contents();
    }

    stepper_steps(false, false, 4000);
    while (!step_done) {
        tight_loop_contents();
    }

    for(;;);
    return 0;
}
