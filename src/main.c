#include <utils.h>
#include <movement.h>
#include <stepper.h>
#include "rf.h"
#include <led.h>
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

    led_init_pwm();
    led_init_isr();

    // int test_idx = 0;

    sleep_ms(2000); // for printing

    while(true){
        data_read = 0;
        data_read = rf_read_data();
        // rf_send_data(data_send);
        // data_send += 1;
        // rx_data = data_recieve(rx_sm, pio);

        // // led and manual stepping test with packets
        // uint8_t curr_addr = ms.x_pos << 4 | ms.y_pos;
        // uint32_t packet_vector[14] = {0};
        // packet_vector[0] = (curr_addr << 24) | (LED_SLOW  << 16) | (0xFF << 8) | (0xFF << 0); // slow white
        // packet_vector[1] = (curr_addr << 24) | (LED_SLOW  << 16) | (0x00 << 8) | (0x00 << 0); // off
        // packet_vector[2] = (curr_addr << 24) | (LED_FAST  << 16) | (0xF8 << 8) | (0x00 << 0); // fast red
        // packet_vector[3] = (curr_addr << 24) | (LED_SLOW  << 16) | (0xFF << 8) | (0xE0 << 0); // slow yellow
        // packet_vector[4] = (curr_addr << 24) | (LED_SLOW  << 16) | (0x07 << 8) | (0xE0 << 0); // slow green
        // // packet_vector[5] = 0xFFFFFFFF;
        // packet_vector[5] = (curr_addr << 24) | (0x06      << 16) | (((int8_t)-50 & 0xFF) << 8) | (((int8_t)127 & 0xFF) << 0); // manual step
        // packet_vector[6] = (curr_addr << 24) | (LED_FAST  << 16) | (0xF8 << 8) | (0x1F << 0); // fast magenta
        // // packet_vector[7] = 0xFFFFFFFF;
        // packet_vector[7] = (curr_addr << 24) | (0x06      << 16) | (((int8_t)50 & 0xFF) << 8) | (((int8_t)-100 & 0xFF) << 0); // manual step
        // packet_vector[8] = (curr_addr << 24) | (LED_SOLID << 16) | (0x00 << 8) | (0x1F << 0); // solid blue
        // packet_vector[9] = (curr_addr << 24) | (0x00      << 16) | (0x02 << 8) | (0x01 << 0); // direct move
        // packet_vector[10] = (curr_addr << 24) | (LED_SOLID << 16) | (0xF8 << 8) | (0x05 << 0); // solid something magentaish
        // packet_vector[11] = (curr_addr << 24) | (0x06      << 16) | (((int8_t)100 & 0xFF) << 8) | (((int8_t)-50 & 0xFF) << 0); // manual step
        // packet_vector[12] = (curr_addr << 24) | (LED_SLOW  << 16) | (0x07 << 8) | (0xE0 << 0); // slow green
        // packet_vector[13] = 0xFFFFFFFF;
        // data_read = 0xFFFFFFFF;
        // int interval = 5;
        // if ((test_idx % interval) == 0) data_read = packet_vector[test_idx / interval];
        // // printf(" | Packet %d: 0x%X\n", test_idx / interval, data_read);
        // if ((test_idx / interval) < 13) test_idx++;

        packet_data.addr = (data_read >> 24 & 0xFF); //{1'b0, x[2:0], 1'b0, y[2:0]}
        packet_data.cmd = (data_read >> 16 & 0xFF);
        packet_data.data1 = (data_read >> 8 & 0xFF);
        packet_data.data2 = (data_read >> 0 & 0xFF);
        if ((ms.x_pos << 4 | ms.y_pos) == packet_data.addr) {
            switch (packet_data.cmd) {
                //direct move
                case 0x00: {
                    stepper_manual(false, 0, 0);
                    move_to(&ms, DIRECT, packet_data.data1 & 0xF, packet_data.data2 & 0xF);
                    break;
                }
                //on-grid move
                case 0x01: {
                    stepper_manual(false, 0, 0);
                    move_to(&ms, ON_GRID, packet_data.data1 & 0xF, packet_data.data2 & 0xF);
                    break;
                }
                //off-grid move
                case 0x02: {
                    stepper_manual(false, 0, 0);
                    move_to(&ms, OFF_GRID, packet_data.data1 & 0xF, packet_data.data2 & 0xF);
                    break;
                }
                //LED solid
                case 0x03: {
                    //{data1, data2} = {red[4:0], green[5:0]], blue[4:0]}
                    //use PWM
                    led_config(LED_SOLID, packet_data.data1, packet_data.data2);
                    break;
                }
                //LED slow blink (2 Hz)
                case 0x04: {
                    //{data1, data2} = {red[4:0], green[5:0]], blue[4:0]}
                    //use PWM
                    led_config(LED_SLOW, packet_data.data1, packet_data.data2);
                    break;
                }
                //LED fast blink (4 Hz)
                case 0x05: {
                    //{data1, data2} = {red[4:0], green[5:0]], blue[4:0]}
                    //use PWM
                    led_config(LED_FAST, packet_data.data1, packet_data.data2);
                    break;
                }
                //Manual movement
                case 0x06: {
                    //right wheel speed = signed'(data1)
                    //left wheel speed = signed'(data2)
                    //speed as a percentage of max speed
                    stepper_manual(true, (int8_t)packet_data.data1, (int8_t)packet_data.data2);
                    break;
                }
                case 0xFF: break; // NOP
            }
        }

        // // led and manual stepping test
        // switch (test_idx) {
        //     case 10: led_config(LED_SLOW, 0xFF, 0xFF); break; // slow white
        //     case 20: led_config(LED_SLOW, 0x00, 0x00); break; // off
        //     case 30: led_config(LED_FAST, 0xF8, 0x00); break; // fast red
        //     case 40: led_config(LED_SLOW, 0xFF, 0xE0); break; // slow yellow
        //     case 50: led_config(LED_FAST, 0x07, 0xE0); break; // fast green
        //     case 60: led_config(LED_SOLID, 0x00, 0x1F); break; // solid blue
        //     case 70: stepper_manual((int8_t)-50, (int8_t)127); break;
        // }
        // test_idx++;

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
