#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include "movement.h"
#include <math.h>

//0x(W/B)(piece) ex: 0x00 is wite castle
#define IDENT 0x00 //IR rx address
#define STARTING_X 0 //starting x position
#define STARTING_Y 0 //starting Y position

static inline uint64_t s_to_us(double seconds) {
    return (uint64_t)(seconds * 1e6);
}

static inline double us_to_s(uint64_t microseconds) {
    return ((double)microseconds) / 1e6;
}

#endif
