#pragma once

#include "eval_types.hpp"

namespace Clockwork {

extern const PScore PAWN_MAT;
extern const PScore KNIGHT_MAT;
extern const PScore BISHOP_MAT;
extern const PScore ROOK_MAT;
extern const PScore QUEEN_MAT;
extern const PScore MOBILITY_VAL;
extern const PScore TEMPO_VAL;
extern const PScore BISHOP_PAIR_VAL;

extern const std::array<PScore, 48> PAWN_PSQT;
extern const std::array<PScore, 64> KNIGHT_PSQT;
extern const std::array<PScore, 64> BISHOP_PSQT;
extern const std::array<PScore, 64> ROOK_PSQT;
extern const std::array<PScore, 64> QUEEN_PSQT;
extern const std::array<PScore, 64> KING_PSQT;

}
