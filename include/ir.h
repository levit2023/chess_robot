#include <stdio.h>
#include "pico/stdlib.h"
#include "nec_transmit.h"
#include "nec_receive.h"

typedef struct nec_config{
    int tx_sm;
    int rx_sm;
}nec_config_t;


struct nec_config ir_init(PIO pio, int tx, int rx);
void ir_send(PIO pio, uint32_t data, int tx_sm);
void ir_receive(PIO pio, int rx_sm);
bool decode_and_check(uint32_t data);
uint32_t encode_data(uint8_t current_pos, uint8_t move);