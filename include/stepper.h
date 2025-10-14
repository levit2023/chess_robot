#ifndef STEPPER_H
#define STEPPER_H

typedef enum {
  STEPPER_ACCEL,
  STEPPER_CRUISE,
  STEPPER_DECEL,
} stepper_state_t;

extern bool stepper_idle;

void stepper_init_pins();
void stepper_init_timer();
void stepper_steps(bool left_fw, bool right_fw, int steps);
void stepper_isr();

#endif
