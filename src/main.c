#include <utils.h>
#include <movement.h>
//0x(W/B)(piece) ex: 0x00 is wite castle
#define IDENT piece //IR rx address

int main()
{
    move_state_t ms;
    chess_piece_t piece = W_KNIGHT_B; //starting position
    move_init(&ms, piece);

    // // Configures our microcontroller to 
    // // communicate over UART through the TX/RX pins
    // stdio_init_all();

    // Configures our microcontroller to
    // communicate UART-over-USB
    // (for flashing without debugger)
    stdio_usb_init();

    // stepper inits
    stepper_init_pins();
    stepper_init_timer();

    move_to(&ms, 2,2);
    // move_to(&ms, 0,0);
    // move_to(&ms, 2,2);
    // move_to(&ms, 2,0);
    // move_to(&ms, 2,2);
    // move_to(&ms, 3,2);
    // move_to(&ms, 0,2);

    for(;;);
    return 0;
}
