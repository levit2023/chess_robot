#ifndef SHARED_H
#define SHARED_H

extern bool step_done;

void stepper_init_pins();
void stepper_init_timer();
void stepper_steps(bool cw, int steps);
void stepper_isr();

#endif
