#pragma once

#include "util/types.hpp"
#include <array>

namespace Clockwork {

class Zobrist {
public:
    Zobrist() = delete;

    static std::array<std::array<std::array<HashKey, 64>, 7>, 2> piece_square_zobrist;
    static std::array<HashKey, 64>                               en_passant_zobrist;
    static std::array<HashKey, 16>                               castling_zobrist;
    static HashKey                                               side_key;

    static void init_zobrist_keys();
};

}  // namespace Clockwork
