#ifndef RF_H
#define RF_H
#include "hardware/spi.h"


void send_spi_cmd(spi_inst_t* spi, uint16_t value);
void send_spi_data(spi_inst_t* spi, uint16_t value);
void rf_send_init_pins();
void rf_read_init_pins();
void rf_send_data();
void rf_recieve_data();
void cd_display1(const char *string);
void cd_display2(const char *string);

#endif