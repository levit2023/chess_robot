#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

static inline uint64_t s_to_us(double seconds) {
    return (uint64_t)(seconds * 1e6);
}

static inline double us_to_s(uint64_t microseconds) {
    return ((double)microseconds) / 1e6;
}

#endif
