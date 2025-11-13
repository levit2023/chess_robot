#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "hardware/adc.h"
#include "hardware/dma.h"

#define JOYSTICK_BTTN 23
#define JOYSTICK_VERT 42
#define JOYSTICK_HORZ 43

void init_adc_freerun() {
    // From SDK docs 5.1.1.

    adc_init();

    // initialize gpio
    adc_gpio_init(JOYSTICK_VERT);
    adc_gpio_init(JOYSTICK_HORZ);

    // start freerunning conversions
    adc_run(true);
}