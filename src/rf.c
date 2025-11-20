#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"
#include "hardware/gpio.h"
#include "rf.h"

// Make sure to set these in main.c
extern const int SPI_SENDRF_SCK; 
extern const int SPI_SENDRF_CSn; 
extern const int SPI_SENDRF_TX;
extern const int SPI_SENDRF_RX;
const int SENDRF_CE;


extern const int SPI_RECIEVERF_SCK; 
extern const int SPI_RECIEVERF_CSn; 
extern const int SPI_RECIEVERF_TX;
extern const int SPI_RECIEVERF_RX;
const int RECIEVERF_CE;

/***************************************************************** */

void rf_send_init_pins() {
    gpio_set_function(SPI_SENDRF_TX, 1);
    gpio_set_function(SPI_SENDRF_RX, 1);
    gpio_set_function(SPI_SENDRF_SCK, 1);
    gpio_set_function(SPI_SENDRF_CSn, 1);
    gpio_init(SENDRF_CE);
    gpio_set_dir(SENDRF_CE, true);
    spi_init(spi0, 250000);
    spi_set_format(spi0, 8, 0, 0, 1);
    hw_set_bits(&spi_get_hw(spi0)->cr1, 2);
}

void rf_read_init_pins() {
    gpio_set_function(SPI_RECIEVERF_TX, 1);
    gpio_set_function(SPI_RECIEVERF_RX, 1);
    gpio_set_function(SPI_RECIEVERF_SCK, 1);
    gpio_set_function(SPI_RECIEVERF_CSn, 1);
    gpio_init(RECIEVERF_CE);
    gpio_set_dir(RECIEVERF_CE, true);
    spi_init(spi1, 250000);
}

void send_spi_cmd(spi_inst_t* spi, uint16_t value) {
    bool busy = spi_is_busy(spi);
    while(busy);
    spi_write16_blocking(spi, &value, 1);
}

// void send_spi_data(spi_inst_t* spi, uint16_t value) {
//     uint16_t data_byte = value | 0x100;
//     send_spi_cmd(spi, data_byte);
// }

void rf_send_data() {
    sleep_ms(1);
    send_spi_cmd(spi0, 0x20);
    sleep_us(40);
    send_spi_cmd(spi0, 0x0C);
    sleep_us(40);
    send_spi_cmd(spi0, 0x25);
    sleep_us(40);
    send_spi_cmd(spi0, 0x6E);
    sleep_us(40);
    send_spi_cmd(spi0, 0x28);
    sleep_us(40);
    send_spi_cmd(spi0, 0x20);
    sleep_us(40);
    send_spi_cmd(spi0, 0x0E);
    sleep_us(40);
    send_spi_cmd(spi0, 0xA0);
    sleep_us(40);
    send_spi_cmd(spi0, 0xAA);
    gpio_put(SENDRF_CE, true);
    sleep_us(12);
    gpio_put(SENDRF_CE, false);
    sleep_us(130);
    gpio_put(SENDRF_CE, true);
    sleep_ms(3);
    gpio_put(SENDRF_CE, false);
}

void rf_recieve_data() {
    

}

void cd_init() {
    sleep_ms(1);
    send_spi_cmd(spi0, 0x3C);
    sleep_us(40);
    send_spi_cmd(spi0, 0xC);
    sleep_us(40);
    send_spi_cmd(spi0, 0x1);
    sleep_ms(2);
    send_spi_cmd(spi0, 0x6);
    sleep_us(40);
}

void cd_display1(const char *str) {
    send_spi_cmd(spi0, 0x80);
    sleep_us(40);
    for(int i = 0; str[i] != '\0'; i++){
        send_spi_data(spi0, str[i]);
    }
}
void cd_display2(const char *str) {
    send_spi_cmd(spi0, 0xC0);
    sleep_us(40);
    for(int i = 0; str[i] != '\0'; i ++){
        send_spi_data(spi0, str[i]);
    }
}

/***************************************************************** */