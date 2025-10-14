#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "movement.h"
#include <math.h>

int main()
{
    // Configures our microcontroller to 
    // communicate over UART through the TX/RX pins
    stdio_init_all();

    // stepper inits
    stepper_init_pins();
    stepper_init_timer();

    // short step test
    stepper_steps(FORWARD, 400);
    while (!stepper_idle) {
        tight_loop_contents();
    }
    stepper_steps(BACKWARD, 400);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    // figure 8 test
    stepper_steps(FORWARD, 2000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(TURN_RIGHT, 1702);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(FORWARD, 2000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(TURN_LEFT, 1702);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(FORWARD, 2000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(TURN_LEFT, 1702);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(FORWARD, 2000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(TURN_RIGHT, 1702);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    stepper_steps(BACKWARD, 4000);
    while (!stepper_idle) {
        tight_loop_contents();
    }

    for(;;);
    return 0;
}
