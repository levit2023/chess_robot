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
    struct movement *path_addr;
    path_addr = find_path(1, 0, 2, 0);

    for (int i = 0; i < 10; i ++){
        printf("Direction: %d, Rotations: %.2lf\n", path_addr[i].dir, path_addr[i].rot);
        stepper_steps(path_addr[i].dir, (int)(path_addr[i].rot * 4096));
        while (!stepper_idle) {
            tight_loop_contents();
        }
    }

    // // short step test
    // stepper_steps(FORWARD, 400);
    // while (!stepper_idle) {
    //     tight_loop_contents();
    // }
    // stepper_steps(BACKWARD, 400);
    // while (!stepper_idle) {
    //     tight_loop_contents();
    // }

    // // P test
    // stepper_steps(FORWARD, 2000);
    // while (!stepper_idle) {
    //     tight_loop_contents();
    // }

    // stepper_steps(TURN_RIGHT, 1702);
    // while (!stepper_idle) {
    //     tight_loop_contents();
    // }

    // stepper_steps(FORWARD, 2000);
    // while (!stepper_idle) {
    //     tight_loop_contents();
    // }

    // stepper_steps(TURN_LEFT, 1702);
    // while (!stepper_idle) {
    //     tight_loop_contents();
    // }

    // stepper_steps(FORWARD, 2000);
    // while (!stepper_idle) {
    //     tight_loop_contents();
    // }

    // stepper_steps(TURN_LEFT, 1702);
    // while (!stepper_idle) {
    //     tight_loop_contents();
    // }

    // stepper_steps(FORWARD, 2000);
    // while (!stepper_idle) {
    //     tight_loop_contents();
    // }

    // stepper_steps(TURN_RIGHT, 1702);
    // while (!stepper_idle) {
    //     tight_loop_contents();
    // }

    // stepper_steps(BACKWARD, 4000);
    // while (!stepper_idle) {
    //     tight_loop_contents();
    // }

    for(;;);
    return 0;
}
