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
    busy_wait_ms(100);

}

uint32_t ir_receive(PIO pio, int rx_sm){
    uint32_t rx_data;
    while (!pio_sm_is_rx_fifo_empty(pio, rx_sm)) {
        rx_data = pio_sm_get(pio, rx_sm);
    }
    return rx_data;
}

ir_data_t decode_and_check(uint32_t rx_data){
    ir_data_t data;
    // union {
    //     uint32_t raw;
    //     struct {
    //         uint8_t currX;
    //         uint8_t inverted_currX;
    //         uint8_t currY;
    //         uint8_t inverted_currY;
    //     };
    // } move_data;

    uint8_t invDest = rx_data >> 24;
    uint8_t dest = (rx_data >> 16) & (0x00ff);
    uint8_t invCurr = (rx_data >> 8) & (0x0000ff);
    uint8_t curr = rx_data & (0x000000ff);

    if((invDest!= (dest ^ 0xff)) ||
    (invCurr != (curr^ 0xff))){
        data.data_valid = false; // Data is invalid
        data.curr_x = 0x0;
        data.curr_y = 0x0;
        data.move_x = 0x0;
        data.move_y = 0x0;
    }
    else{
        data.data_valid = true;
        data.curr_x = curr >> 4;
        data.curr_y = curr & 0xf;
        data.move_x = dest >> 4;
        data.move_y = dest & 0xf;
    }
    return data;
}

uint32_t encode_data(uint8_t curr_position, uint8_t move){

    return (move ^ 0xff) << 24 | move << 16 | (curr_position ^ 0xff) << 8 | curr_position;  

}