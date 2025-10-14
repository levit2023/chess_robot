#include "ir.h"

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
    sleep_ms(100);

}

void ir_receive(PIO pio, int rx_sm){
    while (!pio_sm_is_rx_fifo_empty(pio, rx_sm)) {
        uint32_t rx_data = pio_sm_get(pio, rx_sm);
        if (decode_and_check(rx_data)) {
            // Successful transmission, overwrite previous successful line
            printf("\r\ttransmitted: %02x\n", rx_data);
        }
    }
}

bool decode_and_check(uint32_t rx_data){

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
    (move_data.move != (move_data.inverted_ident ^ 0xff))){
        return true; // Data is invalid
    }
    else{
        return true;
    }
}

uint32_t encode_data(uint8_t ident, uint8_t move){

    return (move ^ 0xff) << 24 | move << 16 | (ident ^ 0xff) << 8 | ident;  

}

