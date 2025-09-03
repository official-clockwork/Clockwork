#pragma once

#include "common.hpp"
#include "position.hpp"

namespace Clockwork {

using MainHistory       = std::array<std::array<std::array<i32, 4>, 4096>, 2>;
using ContHistEntry     = std::array<std::array<std::array<i32, 64>, 6>, 2>;
using ContHistory       = std::array<std::array<std::array<ContHistEntry, 64>, 6>, 2>;
using CorrectionHistory = std::array<std::array<i32, 16384>, 2>;

constexpr i32 HISTORY_MAX                     = 16384;
constexpr u64 CORRECTION_HISTORY_ENTRY_NB     = 16384;
constexpr i32 CORRECTION_HISTORY_GRAIN        = 256;
constexpr i32 CORRECTION_HISTORY_WEIGHT_SCALE = 256;
constexpr i32 CORRECTION_HISTORY_MAX          = CORRECTION_HISTORY_GRAIN * 32;

namespace Search {
struct Stack;
}

class History {
public:
    History() = default;

    ContHistEntry& get_cont_hist_entry(const Position& pos, Move move) {
        usize     stm_idx = static_cast<usize>(pos.active_color());
        PieceType pt      = pos.piece_at(move.from());
        usize     pt_idx  = static_cast<usize>(pt) - static_cast<usize>(PieceType::Pawn);
        return m_cont_hist[stm_idx][pt_idx][move.to().raw];
    }

    i32  get_quiet_stats(const Position& pos, Move move, i32 ply, Search::Stack* ss) const;
    void update_quiet_stats(const Position& pos, Move move, i32 ply, Search::Stack* ss, i32 bonus);

    void clear();

    // Correction history functions
    static void add_correction_history(
      CorrectionHistory& hist, Color side, HashKey pawn_key, i32 depth, i32 diff);
    static i32 get_correction(const CorrectionHistory& hist, Color side, HashKey pawn_key);

private:
    static void update_hist_entry(i32& entry, i32 bonus) {
        entry += bonus - entry * std::abs(bonus) / HISTORY_MAX;
    }

    MainHistory m_main_hist = {};
    ContHistory m_cont_hist = {};
};

}
