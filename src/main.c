#include <utils.h>
#include <movement.h>
#include "ir.h"
//0x(W/B)(piece) ex: 0x00 is wite castle
#define IDENT piece //IR rx address

int main()
{
    move_state_t ms;
    chess_piece_t piece = W_KNIGHT_B; //starting position
    move_init(&ms, piece);
    // Configures our microcontroller to 
    // communicate over UART through the TX/RX pins
    stdio_init_all();
    ir_data_t move_data;
    data_recieve(&move_data);
    while(move_data.piece != piece){
        data_recieve(&move_data);
        sleep_ms(100);
        printf("Recieved: %02x\n", move_data.piece);
    }
    // stepper inits
    stepper_init_pins();
    stepper_init_timer();
    move_to(&ms, move_data.move_x,move_data.move_y);
    // move_to(&ms, 0,0);
    // move_to(&ms, 2,2);
    // move_to(&ms, 2,0);
    // move_to(&ms, 2,2);
    // move_to(&ms, 3,2);
    // move_to(&ms, 0,2);
    for(;;);
    return 0;
}
