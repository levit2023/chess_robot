#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#include "hardware/irq.h"
#include <math.h>

static inline uint64_t s_to_us(double seconds) {
    return (uint64_t)(seconds * 1e6);
}

static inline double us_to_s(uint64_t microseconds) {
    return ((double)microseconds) / 1e6;
}

// typedef enum {
//     WHITE = 0b000000,
//     BLACK = 0b100000,
// }
// typedef enum {
//     ROOK = 0b0000001,
// }

typedef enum {
    //row 1
    B_ROOK_A   = 0x0 << 3 | 0x0 << 0,
    B_KNIGHT_B = 0x1 << 3 | 0x0 << 0,
    B_BISHOP_C = 0x2 << 3 | 0x0 << 0,
    B_QUEEN    = 0x3 << 3 | 0x0 << 0,
    B_KING     = 0x4 << 3 | 0x0 << 0,
    B_BISHOP_F = 0x5 << 3 | 0x0 << 0,
    B_KNIGHT_G = 0x6 << 3 | 0x0 << 0,
    B_ROOK_H   = 0x7 << 3 | 0x0 << 0,
    //row 2
    B_PAWN_A   = 0x0 << 3 | 0x1 << 0,
    B_PAWN_B   = 0x1 << 3 | 0x1 << 0,
    B_PAWN_C   = 0x2 << 3 | 0x1 << 0,
    B_PAWN_D   = 0x3 << 3 | 0x1 << 0,
    B_PAWN_E   = 0x4 << 3 | 0x1 << 0,
    B_PAWN_F   = 0x5 << 3 | 0x1 << 0,
    B_PAWN_G   = 0x6 << 3 | 0x1 << 0,
    B_PAWN_H   = 0x7 << 3 | 0x1 << 0,
    //row 7
    W_PAWN_A   = 0x0 << 3 | 0x6 << 0,
    W_PAWN_B   = 0x1 << 3 | 0x6 << 0,
    W_PAWN_C   = 0x2 << 3 | 0x6 << 0,
    W_PAWN_D   = 0x3 << 3 | 0x6 << 0,
    W_PAWN_E   = 0x4 << 3 | 0x6 << 0,
    W_PAWN_F   = 0x5 << 3 | 0x6 << 0,
    W_PAWN_G   = 0x6 << 3 | 0x6 << 0,
    W_PAWN_H   = 0x7 << 3 | 0x6 << 0,
    //row 8
    W_ROOK_A   = 0x0 << 3 | 0x7 << 0,
    W_KNIGHT_B = 0x1 << 3 | 0x7 << 0,
    W_BISHOP_C = 0x2 << 3 | 0x7 << 0,
    W_QUEEN    = 0x3 << 3 | 0x7 << 0,
    W_KING     = 0x4 << 3 | 0x7 << 0,
    W_BISHOP_F = 0x5 << 3 | 0x7 << 0,
    W_KNIGHT_G = 0x6 << 3 | 0x7 << 0,
    W_ROOK_H   = 0x7 << 3 | 0x7 << 0,
} chess_piece_t;

#endif
