#include <utils.h>
#include <movement.h>
#include "rf.h"
#include "hardware/spi.h"
//0x(W/B)(piece) ex: 0x00 is wite castle
#define IDENT piece //IR rx address
const int SPI_SENDRF_SCK = 22;
const int SPI_SENDRF_CSn = 21;
const int SPI_SENDRF_TX = 19;
const int SPI_SENDRF_RX = 20;
const int SENDRF_CE = 16;

const int SPI_RECIEVERF_SCK = 22;
const int SPI_RECIEVERF_CSn = 21;
const int SPI_RECIEVERF_TX = 19;
const int SPI_RECIEVERF_RX = 20;
const int RECIEVERF_CE = 16;

uint tx_gpio = 18;       // choose which GPIO pin is connected to the IR LED
uint rx_gpio = 12; 

int main()
{
    move_state_t ms;
    chess_piece_t piece = W_ROOK_A; //starting position
    // uint32_t rx_data;
    rf_data_t packet_data;
    stepper_init_pins();
    stepper_init_timer();
    move_init(&ms, piece);
    // stdio_init_all();
    stdio_usb_init();

    int data_read = 0;

    // rf_send_init_pins();
    // rf_send_config();
    // rf_gpio_init_tx();

    rf_read_init_pins();
    rf_recieve_config();
    rf_gpio_init_rx();

    while(true){
        data_read = 0;
        data_read = rf_read_data();
        // rf_send_data(data_send);
        // data_send += 1;
        // rx_data = data_recieve(rx_sm, pio);
        packet_data.addr = (data_read >> 24 & 0xFF); //{1'b0, x[2:0], 1'b0, y[2:0]}
        packet_data.cmd = (data_read >> 16 & 0xFF);
        packet_data.data1 = (data_read >> 8 & 0xFF);
        packet_data.data2 = (data_read >> 0 & 0xFF);
        if (ms.x_pos << 4 | ms.y_pos == packet_data.addr) {
            switch (packet_data.cmd) {
                //direct move
                case 0x0000: {
                    move_to(&ms, DIRECT, packet_data.data1 & 0xF, packet_data.data2 & 0xF);
                    break;
                }
                //on-grid move
                case 0x0001: {
                    move_to(&ms, ON_GRID, packet_data.data1 & 0xF, packet_data.data2 & 0xF);
                    break;
                }
                //off-grid move
                case 0x0002: {
                    move_to(&ms, OFF_GRID, packet_data.data1 & 0xF, packet_data.data2 & 0xF);
                    break;
                }
                //LED solid
                case 0x0003: {
                    //{data1, data2} = {red[4:0], green[5:0]], blue[4:0]}
                    //use PWM
                    break;
                }
                //LED slow blink (2 Hz)
                case 0x0004: {
                    //{data1, data2} = {red[4:0], green[5:0]], blue[4:0]}
                    //use PWM
                    break;
                }
                //LED fast blink (4 Hz)
                case 0x0005: {
                    //{data1, data2} = {red[4:0], green[5:0]], blue[4:0]}
                    //use PWM
                    break;
                }
                //Manual movement
                case 0x0006: {
                    //right wheel speed = signed'(data1)
                    //left wheel speed = signed'(data2)
                    //speed as a percentage of max speed
                    break;
                }
            }
        }

        // printf("\r\tRecieved packet: %02x\n", data_read);
        // printf("\r\tRecieved currX: %02x\n", move_data.curr_x);
        // printf("\r\tRecieved currY: %02x\n", move_data.curr_y);
        // printf("\r\tRecieved destX: %02x\n", move_data.move_x);
        // printf("\r\tRecieved destY: %02x\n", move_data.move_y);

        // printf("\r\tRecieved msX: %02x\n", ms.x_pos);
        // printf("\r\tRecieved msY: %02x\n", ms.y_pos);
        // fflush(stdout);
        // if(move_data.data_valid){
        //     if(move_data.piece == 0x08){
        //         printf("\r\tRecieved: %02x", move_data.piece);
        //         fflush(stdout);
        //     }
        //     else{
        //         printf("\r\tPass Parity Error\n: %02x", move_data.piece);
        //     }
        // // }
        sleep_ms(200);
        // data_read = rf_read_data();
        printf("\r\tRecieved: %08x", data_read);
        fflush(stdout);

    }


    // stepper inits
    // stepper_init_pins();
    // stepper_init_timer();
    // move_to(&ms, move_data.move_x,move_data.move_y);
    // move_to(&ms, 0,0);
    // move_to(&ms, 2,2);
    // move_to(&ms, 2,0);
    // move_to(&ms, 2,2);
    // move_to(&ms, 3,2);
    // move_to(&ms, 0,2);
    for(;;);
    return 0;
}
