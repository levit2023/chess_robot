/**
 * Copyright (c) 2021 mjcross
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"

#include "nec_transmit.h"                           // include the library headers
#include "nec_receive.h"
#include "ir.h"

// Infrared loopback example ('NEC' format)
//
// Need to connect an IR LED to GPIO 14 via a suitable series resistor (e.g. 1.5k)
// and an active-low IR detector to GPIO 15 (e.g. VS1838b)
//
// Output is sent to stdout

uint tx_gpio = 18;
uint rx_gpio = 30;
PIO pio = pio0;

int main() {
    stdio_init_all();

    nec_config_t my_setup;

    my_setup = ir_init(pio, tx_gpio, rx_gpio);
    uint32_t tx = encode_data(0x12, 0x00);
    ir_send(pio, tx, my_setup.tx_sm);
    ir_receive(pio, my_setup.rx_sm);

}