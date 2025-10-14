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

    // stepper inits
    stepper_init_pins();
    stepper_init_timer();

    // pending: change left_fw right_fw to forward, backward, left_turn, right_turn
    
    // short step test
    stepper_steps(true, true, 400);
    while (!stepper_idle) {
        tight_loop_contents();
    }
    stepper_steps(false, false, 400);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    // figure 8 test
    stepper_steps(true, true, 2000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(true, false, 2000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(true, true, 2000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(false, true, 2000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(true, true, 2000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(false, true, 2000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(true, true, 2000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(true, false, 2000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(false, false, 4000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    for(;;);
    return 0;
}
