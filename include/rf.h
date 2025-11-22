#ifndef RF_H
#define RF_H
#include "hardware/spi.h"


void send_spi_cmd(spi_inst_t* spi, uint16_t value);
void send_spi_data(spi_inst_t* spi, uint16_t value);
void rf_send_init_pins();
void rf_read_init_pins();
void spi_send_cmd_tx(int data_size, uint16_t data);
void spi_send_cmd_rx(int data_size, uint16_t data);
void rf_send_config();
void rf_send_data();
void rf_recieve_config();
void rf_read_data();
// void cd_display1(const char *string);
// void cd_display2(const char *string);

#endif