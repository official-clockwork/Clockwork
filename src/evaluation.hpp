#pragma once

#include "position.hpp"

#include "eval_types.hpp"

namespace Clockwork {
extern const PScore PAWN_MAT;
extern const PScore KNIGHT_MAT;
extern const PScore BISHOP_MAT;
extern const PScore ROOK_MAT;
extern const PScore QUEEN_MAT;
extern const PScore MOBILITY_VAL;
extern const PScore TEMPO_VAL;
Score               evaluate(Position pos);
};
