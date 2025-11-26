#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdbool.h>
#include "utils.h"

typedef enum {
  STEPPER_ACCEL,
  STEPPER_CRUISE,
  STEPPER_DECEL,
} stepper_state_t;

typedef enum {
  FORWARD = 0,
  BACKWARD = 1,
  TURN_LEFT = 2,
  TURN_RIGHT = 3
} direction_t;

typedef struct move_state {
  int x_pos;
  int y_pos;
} move_state_t;

typedef enum {
  DIRECT = 0,
  ON_GRID = 1,
  OFF_GRID = 2
} pathfind_mode_t;

struct movement {
  direction_t dir; //direction to move in
  double rot; //number of rotations
};

extern bool stepper_idle;

void stepper_init_pins();
void stepper_init_timer();
void stepper_steps(direction_t dir, int steps);
void stepper_isr();

struct movement *find_path(pathfind_mode_t mode, int pos_x, int pos_y, int set_x, int set_y);

void move_to(move_state_t * state, pathfind_mode_t mode, int x, int y);
void move_init(move_state_t * state, chess_piece_t chess_piece);


#endif
