#ifndef RF_H
#define RF_H
#include "hardware/spi.h"

void send_spi_cmd(spi_inst_t* spi, uint16_t value, int size);
void send_spi_data(spi_inst_t* spi, uint16_t value);
void rf_send_init_pins();
void rf_read_init_pins();
void spi_send_cmd_tx(int data_size, uint16_t data);
void spi_send_cmd_rx(int data_size, uint16_t data);
void rf_send_config();
void rf_send_data();
void rf_recieve_config();
int rf_read_data();
void rf_gpio_init_tx();
void rf_gpio_init_rx();
void rf_bit_bang_tx(int data, int data_size);
int rf_bit_bang_rx(int data, int data_size);
void rf_init_recieve_irq();
void recieve_isr();

typedef struct rf_data{
    uint8_t addr;
    uint8_t cmd;
    uint8_t data1;
    uint8_t data2;
} rf_data_t;

#endif