#ifndef SHARED_H
#define SHARED_H

#include <movement.h>

extern bool step_done;

void stepper_init_pins();
void stepper_init_timer();
void stepper_steps(direction_t dir, int steps);
void stepper_steps_isr();
void stepper_manual_right_isr();
void stepper_manual_left_isr();
void stepper_manual(bool en, int8_t data1, int8_t data2);

#endif
