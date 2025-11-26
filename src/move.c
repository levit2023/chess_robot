#include "movement.h"
#include "utils.h"
#include <stdio.h>

void move_init(move_state_t * state, chess_piece_t chess_piece) {
  state->x_pos = (chess_piece >> 3) & 0x7;
  state->y_pos = (chess_piece >> 0) & 0x7;
}

void move_to(move_state_t * state, pathfind_mode_t mode, int x, int y) {
    struct movement *path_addr;
    path_addr = find_path(mode, state->x_pos, state->y_pos, x, y);

    for (int i = 0; i < 10; i ++){
        printf("Direction: %d, Rotations: %.2lf\n", path_addr[i].dir, path_addr[i].rot);
        stepper_steps(path_addr[i].dir, (int)(path_addr[i].rot * 4096));
        while (!stepper_idle) {
            tight_loop_contents();
        }
    }
    state->x_pos = x;
    state->y_pos = y;
}