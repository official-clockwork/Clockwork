#pragma once

#include "position.hpp"

#include "eval_types.hpp"

namespace Clockwork {
#ifndef EVAL_TUNING
static inline PScore PAWN_MAT     = S(100, 100);
static inline PScore KNIGHT_MAT   = S(330, 330);
static inline PScore BISHOP_MAT   = S(370, 370);
static inline PScore ROOK_MAT     = S(550, 550);
static inline PScore QUEEN_MAT    = S(1000, 1000);
static inline PScore MOBILITY_VAL = S(10, 10);
static inline PScore TEMPO_VAL    = S(0, 0);
#else
PScore PAWN_MAT     = S(100, 100);
PScore KNIGHT_MAT   = S(330, 330);
PScore BISHOP_MAT   = S(370, 370);
PScore ROOK_MAT     = S(550, 550);
PScore QUEEN_MAT    = S(1000, 1000);
PScore MOBILITY_VAL = S(10, 10);
PScore TEMPO_VAL    = S(0, 0);
#endif
Score evaluate(Position pos);
};
