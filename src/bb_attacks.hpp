#pragma once

#include "bitboard.hpp"
#include "common.hpp"
#include "square.hpp"
#include "util/types.hpp"

namespace Clockwork {

[[nodiscard]] Bitboard pawn_attacks(Square square, Color color);
[[nodiscard]] Bitboard knight_attacks(Square square);
[[nodiscard]] Bitboard bishop_attacks(Square square, Bitboard occupancy);
[[nodiscard]] Bitboard rook_attacks(Square square, Bitboard occupancy);
[[nodiscard]] Bitboard queen_attacks(Square square, Bitboard occupancy);
[[nodiscard]] Bitboard king_attacks(Square square);

}
