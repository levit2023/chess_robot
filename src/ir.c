#include "ir.h"


uint tx_gpio = 18;
uint rx_gpio = 30;
PIO pio = pio0;
int times_to_send = 10;

struct nec_config ir_init(){ // Initialize PIO state machines

    struct nec_config config;

    config.tx_sm = nec_tx_init(pio, tx_gpio);
    config.rx_sm = nec_rx_init(pio, rx_gpio);

    if(config.tx_sm == -1 || config.rx_sm == -1){
        printf("Error did not configure state machine");
    }
    return config;
}

void ir_send(uint32_t data, int tx_sm){
    
    pio_sm_put(pio, tx_sm, data);
    sleep_ms(200);

}

void recieve_data(uint32_t rx_data, int rx_sm){

    rx_data = pio_sm_get(pio, rx_sm);
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
    if((move_data.ident != ~move_data.inverted_ident) ||
    (move_data.move != ~move_data.inverted_ident)){
        return true; // Data is valid
    }
    else{
        return false;
    }
}

uint32_t encode_data(uint8_t ident, uint8_t move){

    return (~move << 24) | (move << 16) | (~ident << 8) | (ident);  

}

