#include <stdio.h>
#include "pico/stdlib.h"
#include "nec_transmit.h"
#include "nec_receive.h"

struct nec_config{
    int tx_sm;
    int rx_sm;
};


struct nec_config ir_init();
void send_data(uint32_t data, int tx_sm);
void recieve_data(uint32_t data, int tx_sm);
bool decode_and_check(uint32_t data);
uint32_t encode_data(uint8_t ident, uint8_t move);