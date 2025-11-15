#include <stdio.h>
#include "pico/stdlib.h"
#include "nec_transmit.h"
#include "nec_receive.h"

typedef struct nec_config{
    int tx_sm;
    int rx_sm;
}nec_config_t;

typedef struct ir_data{
    uint8_t curr_x;
    uint8_t curr_y;
    uint8_t move_x;
    uint8_t move_y;
    bool data_valid;
} ir_data_t;


struct nec_config ir_init(PIO pio, int tx, int rx);
void ir_send(PIO pio, uint32_t data, int tx_sm);
uint32_t ir_receive(PIO pio, int rx_sm);
ir_data_t decode_and_check(uint32_t data);
uint32_t encode_data(uint8_t ident, uint8_t move);
void data_send(int tx_sm, PIO pio, uint8_t address, uint8_t move);
uint32_t data_recieve(int rx_sm, PIO pio);