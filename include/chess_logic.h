#include "pico/stdlib.h"
#include "hardware/spi.h"

void draw_piece(uint16_t bitmap[], int index, int x_coord, int y_coord);
void draw_square(uint16_t drawn_piece, int board_x, int board_y, bool selected);
void draw_captured(uint16_t drawn_piece, float board_x, float board_y, bool selected);
void draw_board(uint8_t board[8][8]);
void draw_legal_moves();
bool check_legal_move(int move_x, int move_y);
void delete_list(int list_type);
void clear_legal_moves();
bool find_legal_move();
bool find_legal_move_coord(int x, int y);
void add_move(uint16_t piece_id, uint16_t x_coord, uint16_t y_coord, uint8_t list_type);
void legal_move_generator(bool checkmate_assessment);
void init_spi_lcd();
bool stalemate();
bool checkmate();
void gpio_chess_logic_isr();
void init_gpio_chess_logic();
void board_setup();
