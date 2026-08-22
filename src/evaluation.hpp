#pragma once

#include "eval_types.hpp"
#include "position.hpp"
#include "psqt_state.hpp"
#include <array>

namespace Clockwork {

Score evaluate_white_pov(const Position& pos, const PsqtState& psqt_state);
Score evaluate_stm_pov(const Position& pos, const PsqtState& psqt_state);

inline Score evaluate_white_pov(const Position& pos) {
    return evaluate_white_pov(pos, PsqtState{pos});
}

inline Score evaluate_stm_pov(const Position& pos) {
    return evaluate_stm_pov(pos, PsqtState{pos});
}

static constexpr std::array<std::array<Bitboard, 8>, 2> king_flank = []() {
    std::array<std::array<Bitboard, 8>, 2> result{};

    // Queenside files (0-2), Center files (3-4), Kingside files (5-7)
    constexpr std::array<i32, 8> flank_start = {0, 0, 0, 2, 2, 4, 4, 4};
    constexpr std::array<i32, 8> flank_width = {4, 4, 4, 4, 4, 4, 4, 4};
    constexpr Bitboard           white_ranks = Bitboard::rank_mask(0) | Bitboard::rank_mask(1)
                                   | Bitboard::rank_mask(2) | Bitboard::rank_mask(3)
                                   | Bitboard::rank_mask(4);
    constexpr Bitboard black_ranks = Bitboard::rank_mask(3) | Bitboard::rank_mask(4)
                                   | Bitboard::rank_mask(5) | Bitboard::rank_mask(6)
                                   | Bitboard::rank_mask(7);

    for (usize file_idx = 0; file_idx < 8; file_idx++) {
        Bitboard flank{};
        for (i32 f = flank_start[file_idx]; f < flank_start[file_idx] + flank_width[file_idx];
             f++) {
            flank |= Bitboard::file_mask(f);
        }

        result[0][file_idx] = flank & white_ranks;
        result[1][file_idx] = flank & black_ranks;
    }

    return result;
}();


};  // namespace Clockwork
