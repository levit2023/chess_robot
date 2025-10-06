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
    stepper_steps(true, 8192, 10); // 4096 is one revolution
    while (!step_done) {
        tight_loop_contents();
    }
    stepper_steps(false, 8192, 10);

    for(;;);
    return 0;
}
