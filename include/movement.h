#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdbool.h>

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

struct movement {
  direction_t dir; //direction to move in
  double rot; //number of rotations
};

extern bool stepper_idle;

void stepper_init_pins();
void stepper_init_timer();
void stepper_steps(direction_t dir, int steps);
void stepper_isr();

struct movement *find_path(int pos_x, int pos_y, int set_x, int set_y);

#endif
