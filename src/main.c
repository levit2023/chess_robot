#include <utils.h>
#include <movement.h>
#include "ir.h"
#include "hardware/spi.h"
//0x(W/B)(piece) ex: 0x00 is wite castle
#define IDENT piece //IR rx address
const int SPI_SENDRF_SCK = 34;
const int SPI_SENDRF_CSn = 33;
const int SPI_SENDRF_TX = 35;
const int SPI_SENDRF_RX = 36;
const int SENDRF_CE = 27;

const int SPI_RECIEVERF_SCK = 42;
const int SPI_RECIEVERF_CSn = 41;
const int SPI_RECIEVERF_TX = 43;
const int SPI_RECIEVERF_RX = 44;
const int RECIEVERF_CE = 47;

PIO pio = pio0;          // choose which PIO block to use (RP2040 has two: pio0 and pio1)
uint tx_gpio = 18;       // choose which GPIO pin is connected to the IR LED
uint rx_gpio = 12; 

int main()
{
    // move_state_t ms;
    // chess_piece_t piece = B_KNIGHT_B; //starting position
    // uint32_t rx_data;
    // ir_data_t move_data;
    // stepper_init_pins();
    // stepper_init_timer();
    // move_init(&ms, piece);
    // // stdio_init_all();
    // stdio_usb_init();
    // //int tx_sm = nec_tx_init(pio, tx_gpio);
    // int rx_sm = nec_rx_init(pio, rx_gpio); 
    // Configures our microcontroller to 
    // communicate over UART through the TX/RX pins
    rf_send_init_pins();

    while(true){
        rf_send_data();
        // rx_data = data_recieve(rx_sm, pio);
        // move_data = decode_and_check(rx_data);
        // if((ms.x_pos == move_data.curr_x) && (ms.y_pos == move_data.curr_y)){
        //     move_to(&ms, move_data.move_x, move_data.move_y);
        // }
        // printf("\r\tRecieved packet: %02x\n", rx_data);
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
        // }
        sleep_ms(200);
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
