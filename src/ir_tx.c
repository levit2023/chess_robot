#include "hardware/pio.h"
#include "ir_tx.pio.h"

#define IR_PIN 10

void ir_send(uint32_t msg) {
    while (pio_sm_is_tx_fifo_full(pio0, 0));
    pio_sm_put_blocking(pio0, 0, msg);
}

int main() {
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &ir_tx_program);
    ir_tx_program_init(pio, sm, offset, IR_PIN);
    while (true) {
        ir_send(0xA90); // example message
        sleep_ms(1000);
    }
}
