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
    spi_set_format(spi0, 16, 0, 0, 1);
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
    spi_set_format(spi1, 16, 0, 0, 1);
    hw_set_bits(&spi_get_hw(spi1)->cr1, 2);
}

void send_spi_cmd(spi_inst_t* spi, uint16_t value, int size) {
    spi_set_format(spi0, 8*size, 0, 0, 1);
    hw_set_bits(&spi_get_hw(spi0)->cr1, 2);
    bool busy = spi_is_busy(spi);
    while(busy);
    spi_write16_blocking(spi, &value, 1);
}

void rf_send_config() {
    sleep_ms(1);
    send_spi_cmd(spi0, 0x200E, 2);
    sleep_us(40);
    send_spi_cmd(spi0, 0x2100, 2);
    sleep_us(40);
    send_spi_cmd(spi0, 0x2303, 2);
    sleep_us(40);
    send_spi_cmd(spi0, 0x2400, 2);
    sleep_us(40);
    send_spi_cmd(spi0, 0x256E, 2);
    sleep_us(40);
    send_spi_cmd(spi0, 0x2626, 2);
    sleep_us(40);
    send_spi_cmd(spi0, 0x3C00, 2);
    // send_spi_cmd(spi0, 0x2400);
    // sleep_us(40);
    // send_spi_cmd(spi0, 0x256E);
    // sleep_us(40);
    // send_spi_cmd(spi0, 0x2626);
}

void rf_send_data(int data){
    sleep_ms(40);
    rf_bit_bang_tx(0xABCDEF01, 4);
    // send_spi_cmd(spi0, 0x0000, 5);
    // send_spi_cmd(spi0, 0xA0AA);
    // sleep_us(40);
    // // send_spi_cmd(spi0, 0x0E);
    // // sleep_us(40);
    // // send_spi_cmd(spi0, 0xA0);
    // // sleep_us(40);
    // // send_spi_cmd(spi0, 0xAA);
    // gpio_put(SENDRF_CE, true);
    // sleep_us(10);
    // gpio_put(SENDRF_CE, false);
    // sleep_us(130);
    // gpio_put(SENDRF_CE, true);
    // sleep_ms(3);
    // gpio_put(SENDRF_CE, false);
}

void rf_recieve_config() {
    sleep_ms(1);
    send_spi_cmd(spi1, 0x200F, 2);
    sleep_us(40);
    send_spi_cmd(spi1, 0x2100, 2);
    sleep_us(40);
    send_spi_cmd(spi1, 0x2201, 2);
    sleep_us(40);
    send_spi_cmd(spi1, 0x2303, 2);
    sleep_us(40);
    send_spi_cmd(spi1, 0x256E, 2);
    sleep_us(40);
    send_spi_cmd(spi1, 0x2626, 2);
    sleep_us(40);
    send_spi_cmd(spi1, 0x3104, 2);
}

int rf_read_data(){
    int data_read;
    sleep_us(40);
    gpio_put(RECIEVERF_CE, true);
    sleep_us(130);
    sleep_ms(4);
    data_read = rf_bit_bang_rx(0x00000000, 4);
    return data_read;
    gpio_put(RECIEVERF_CE, false);
    // send_spi_cmd(spi1, 0x6100, 2);
    // sleep_ms(40);
    // gpio_put(RECIEVERF_CE, false);
}

void rf_gpio_init_tx(){
    gpio_init(SPI_SENDRF_CSn);
    gpio_init(SPI_SENDRF_RX);
    gpio_init(SPI_SENDRF_TX);
    gpio_init(SPI_SENDRF_SCK);
    gpio_set_dir(SPI_SENDRF_CSn, true);
    gpio_set_dir(SPI_SENDRF_TX, true);
    gpio_set_dir(SPI_SENDRF_SCK, true);
    gpio_set_dir(SPI_SENDRF_RX, false);
    gpio_put(SPI_SENDRF_CSn, true);
}

void rf_gpio_init_rx(){
    gpio_init(SPI_RECIEVERF_CSn);
    gpio_init(SPI_RECIEVERF_RX);
    gpio_init(SPI_RECIEVERF_TX);
    gpio_init(SPI_RECIEVERF_SCK);
    gpio_set_dir(SPI_RECIEVERF_CSn, true);
    gpio_set_dir(SPI_RECIEVERF_TX, true);
    gpio_set_dir(SPI_RECIEVERF_SCK, true);
    gpio_set_dir(SPI_RECIEVERF_RX, false);
    gpio_put(SPI_RECIEVERF_CSn, true);

}

void rf_bit_bang_tx(int data, int data_size){

    bool zero_or_one;
    gpio_put(SPI_SENDRF_CSn, false);
    sleep_us(10);
    for(int j = 7; j >= 0; j--){
        zero_or_one = (0xB000 & (1u << (j+8)));
        gpio_put(SPI_SENDRF_TX, zero_or_one);
        sleep_us(1);
        gpio_put(SPI_SENDRF_SCK, true);
        sleep_us(5);
        gpio_put(SPI_SENDRF_SCK, false);
        sleep_us(5);
    }
    for(int i = (data_size * 8) - 1; i >= 0; i--){
        zero_or_one = (data & (1u << i));
        gpio_put(SPI_SENDRF_TX, zero_or_one);
        sleep_us(1);
        gpio_put(SPI_SENDRF_SCK, true);
        sleep_us(5);
        gpio_put(SPI_SENDRF_SCK, false);
        sleep_us(5);
    }
    gpio_put(SPI_SENDRF_CSn, true);
    sleep_us(10);
    gpio_put(SENDRF_CE, true);
    sleep_us(10);
    gpio_put(SENDRF_CE, false);
    sleep_us(130);
    gpio_put(SENDRF_CE, true);
    sleep_ms(4);
    gpio_put(SENDRF_CE, false);
}

int rf_bit_bang_rx(int data, int data_size){
    int data_read = 0;
    bool gpio_read;
    bool zero_or_one;
    gpio_put(SPI_RECIEVERF_CSn, false);
    sleep_us(10);
    for(int j = 7; j >= 0; j--){
        zero_or_one = (0x6100 & (1u << (j+8)));
        gpio_put(SPI_RECIEVERF_TX, zero_or_one);
        sleep_us(1);
        gpio_put(SPI_RECIEVERF_SCK, true);
        sleep_us(5);
        gpio_put(SPI_RECIEVERF_SCK, false);
        sleep_us(5);
    }
    for(int i = (data_size * 8) - 1; i >= 0; i--){
        zero_or_one = (data & (1u << i));
        gpio_put(SPI_RECIEVERF_TX, zero_or_one);
        sleep_us(1);
        gpio_put(SPI_RECIEVERF_SCK, true);
        gpio_read = gpio_get(SPI_RECIEVERF_RX);
        data_read += (gpio_read << i);
        sleep_us(5);
        gpio_put(SPI_RECIEVERF_SCK, false);
        sleep_us(5);
    }
    gpio_put(SPI_RECIEVERF_CSn, true);
    sleep_us(10);
    for(int i = 7; i >= 0; i--){
        zero_or_one = (0xC200 & (1u << (i+8)));
        gpio_put(SPI_RECIEVERF_TX, zero_or_one);
        sleep_us(1);
        gpio_put(SPI_RECIEVERF_SCK, true);
        sleep_us(5);
        gpio_put(SPI_RECIEVERF_SCK, false);
        sleep_us(5);
    }
    return data_read;
}

/***************************************************************** */