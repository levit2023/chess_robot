// /**
//  * Copyright (c) 2021 mjcross
//  *
//  * SPDX-License-Identifier: BSD-3-Clause
//  */

// #include <stdio.h>
// #include "pico/stdlib.h"

// #include "nec_transmit.h"                           // include the library headers
// #include "nec_receive.h"
// #include "ir.h"

// // Infrared loopback example ('NEC' format)
// //
// // Need to connect an IR LED to GPIO 14 via a suitable series resistor (e.g. 1.5k)
// // and an active-low IR detector to GPIO 15 (e.g. VS1838b)
// //
// // Output is sent to stdout



// void data_send() {
//     stdio_init_all();

//     ir_data_t move_data;

//     // nec_config_t my_setup;

//     // my_setup = ir_init(pio, tx_gpio, rx_gpio);
//     // uint32_t tx = encode_data(0x12, 0x00);

//     // configure and enable the state machines
//     int tx_sm = nec_tx_init(pio, tx_gpio);         // uses two state machines, 16 instructions and one IRQ
//     //int rx_sm = nec_rx_init(pio, rx_gpio);         // uses one state machine and 9 instructions

//     if (tx_sm == -1) {
//         printf("could not configure PIO\n");
//         return -1;
//     }

//     // transmit and receive frames
//     uint8_t tx_address = 0x34, tx_data = 0x12;
//     uint32_t tx_frame = nec_encode_frame(tx_address, tx_data);
//     pio_sm_put(pio, tx_sm, tx_frame);
//     sleep_ms(100);
//     // uint32_t rx_data = ir_receive(pio, rx_sm);
//     // decode_and_check(rx_data, &move_data);
//     // printf("Recieved: %02x\n", move_data.piece);

// }