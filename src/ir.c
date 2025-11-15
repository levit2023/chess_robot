#include "ir.h"
#include <stdio.h>
#include "pico/stdlib.h"

#include "nec_transmit.h"                           // include the library headers
#include "nec_receive.h"



struct nec_config ir_init(PIO pio, int tx, int rx){ // Initialize PIO state machines

    struct nec_config config;

    config.tx_sm = nec_tx_init(pio, tx);
    config.rx_sm = nec_rx_init(pio, rx);

    if(config.tx_sm == -1 || config.rx_sm == -1){
        printf("Error did not configure state machine\n");
    }
    return config;
}

void ir_send(PIO pio, uint32_t data, int tx_sm){
    
    pio_sm_put(pio, tx_sm, data);

}

uint32_t ir_receive(PIO pio, int rx_sm){
        uint32_t rx_frame;
        while (!pio_sm_is_rx_fifo_empty(pio, rx_sm)) {
            rx_frame = pio_sm_get(pio, rx_sm);
        }
        return rx_frame;
}

ir_data_t decode_and_check(uint32_t rx_data){
    ir_data_t data;
    union {
        uint32_t raw;
        struct {
            uint8_t ident;
            uint8_t inverted_ident;
            uint8_t move;
            uint8_t inverted_move;
        };
    } move_data;

    move_data.raw = rx_data;
    if((move_data.ident != (move_data.inverted_ident ^ 0xff)) ||
    (move_data.move != (move_data.inverted_move ^ 0xff))){
        data.data_valid = false; // Data is invalid
        data.curr_x = 0x0;
        data.curr_y = 0x0;
        data.move_x = 0x0;
        data.move_y = 0x0;
    }
    else{
        data.data_valid = true;
        data.curr_x = (move_data.ident >> 3) & 0x7;
        data.curr_y = (move_data.ident & 0x7);
        data.move_x = (move_data.move >> 3) & (0x7);
        data.move_y = (move_data.move & 0x7);
    }
    return data;
}

uint32_t encode_data(uint8_t ident, uint8_t move){

    return (move ^ 0xff) << 24 | move << 16 | (ident ^ 0xff) << 8 | ident;  

}

void data_send(int tx_sm, PIO pio, uint8_t address, uint8_t move){

    

    // nec_config_t my_setup;

    // my_setup = ir_init(pio, tx_gpio, rx_gpio);
    // uint32_t tx = encode_data(0x12, 0x00);

    // configure and enable the state machines
    // uses two state machines, 16 instructions and one IRQ
    //int rx_sm = nec_rx_init(pio, rx_gpio);         // uses one state machine and 9 instructions

    if (tx_sm == -1) {
        printf("could not configure PIO\n");
        return;
    }

    // transmit and receive frames
    uint32_t tx_frame = nec_encode_frame(address, move);
    pio_sm_put(pio, tx_sm, tx_frame);
    sleep_ms(100);
}


// TODO: an init function later
uint32_t  data_recieve(int rx_sm, PIO pio){
    uint32_t rx_data = ir_receive(pio, rx_sm);
}

