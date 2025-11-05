#pragma once

#include "common.hpp"
#include "position.hpp"
#include <algorithm>

namespace Clockwork {

using MainHistory   = std::array<std::array<std::array<i32, 4>, 4096>, 2>;
using ContHistEntry = std::array<std::array<std::array<i32, 64>, 6>, 2>;
using ContHistory   = std::array<std::array<std::array<ContHistEntry, 64>, 6>, 2>;
// king can't get captured
using CaptHistory       = std::array<std::array<std::array<std::array<i32, 64>, 6>, 6>, 2>;
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

    i32  get_conthist(const Position& pos, Move move, i32 ply, Search::Stack* ss) const;
    i32  get_quiet_stats(const Position& pos, Move move, i32 ply, Search::Stack* ss) const;
    void update_cont_hist(const Position& pos, Move move, i32 ply, Search::Stack* ss, i32 bonus);
    void update_quiet_stats(const Position& pos, Move move, i32 ply, Search::Stack* ss, i32 bonus);

    i32  get_noisy_stats(const Position& pos, Move move) const;
    void update_noisy_stats(const Position& pos, Move move, i32 bonus);

    void update_correction_history(const Position& pos, i32 depth, i32 diff);
    i32  get_correction(const Position& pos);

    void clear();

private:
    static void update_hist_entry(i32& entry, i32 bonus) {
        entry += bonus - entry * std::abs(bonus) / HISTORY_MAX;
    }

    static void update_hist_entry_banger(i32& entry, i32 base, i32 bonus) {
        entry += bonus - base * std::abs(bonus) / HISTORY_MAX;
        entry = std::clamp(entry, -2 * HISTORY_MAX, 2 * HISTORY_MAX);
    }

    MainHistory                      m_main_hist          = {};
    ContHistory                      m_cont_hist          = {};
    CaptHistory                      m_capt_hist          = {};
    CorrectionHistory                m_pawn_corr_hist     = {};
    std::array<CorrectionHistory, 2> m_non_pawn_corr_hist = {};
    CorrectionHistory                m_major_corr_hist    = {};
    CorrectionHistory                m_minor_corr_hist    = {};
};

}  // namespace Clockwork
