// #include <utils.h>
// #include <movement.h>
// //0x(W/B)(piece) ex: 0x00 is wite castle
// #define IDENT piece //IR rx address

// int main()
// {
//     move_state_t ms;
//     chess_piece_t piece = W_KNIGHT_B; //starting position
//     move_init(&ms, piece);
//     // Configures our microcontroller to 
//     // communicate over UART through the TX/RX pins
//     stdio_init_all();

// //     // stepper inits
// //     stepper_init_pins();
// //     stepper_init_timer();

//     move_to(&ms, 2,2);
//     // move_to(&ms, 0,0);
//     // move_to(&ms, 2,2);
//     // move_to(&ms, 2,0);
//     // move_to(&ms, 2,2);
//     // move_to(&ms, 3,2);
//     // move_to(&ms, 0,2);

// // //     for(;;);
// // //     return 0;
// }
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>
#include <math.h>   
#include "pieces.h"
#include "chess_logic.h"

/****************************************** */
#define PIN_SDI    19
#define PIN_CS     17
#define PIN_SCK    18
#define PIN_DC     16
#define PIN_nRESET 15

int main() {
    stdio_init_all();
    init_gpio_chess_logic();
    init_spi_lcd();
    board_setup();

    for(;;);
}