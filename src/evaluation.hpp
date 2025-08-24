#pragma once

#include "position.hpp"

#include "eval_types.hpp"

namespace Clockwork {
PScore PAWN_MAT     = S(100, 100);
PScore KNIGHT_MAT   = S(330, 330);
PScore BISHOP_MAT   = S(370, 370);
PScore ROOK_MAT     = S(550, 550);
PScore QUEEN_MAT    = S(1000, 1000);
PScore MOBILITY_VAL = S(10, 10);
PScore TEMPO_VAL    = S(0, 0);

Score evaluate(Position pos);
};
