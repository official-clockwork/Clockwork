#include "history.hpp"
#include "search.hpp"

namespace Clockwork {

i32 History::get_quiet_stats(const Position& pos, Move move, i32 ply, Search::Stack* ss) const {
    auto  to_attacked   = pos.attack_table(~pos.active_color()).read(move.to()) != 0;
    auto  from_attacked = pos.attack_table(~pos.active_color()).read(move.from()) != 0;
    usize stm_idx       = static_cast<usize>(pos.active_color());
    i32   stats         = m_main_hist[static_cast<usize>(pos.active_color())][move.from_to()]
                           [from_attacked * 2 + to_attacked];

    PieceType pt     = pos.piece_at(move.from());
    usize     pt_idx = static_cast<usize>(pt) - static_cast<usize>(PieceType::Pawn);
    if (ply >= 1 && (ss - 1)->cont_hist_entry != nullptr) {
        stats += (*(ss - 1)->cont_hist_entry)[stm_idx][pt_idx][move.to().raw];
    }
    if (ply >= 2 && (ss - 2)->cont_hist_entry != nullptr) {
        stats += (*(ss - 2)->cont_hist_entry)[stm_idx][pt_idx][move.to().raw];
    }

    return stats;
}

void History::update_quiet_stats(
  const Position& pos, Move move, i32 ply, Search::Stack* ss, i32 bonus) {
    auto  to_attacked   = pos.attack_table(~pos.active_color()).read(move.to()) != 0;
    auto  from_attacked = pos.attack_table(~pos.active_color()).read(move.from()) != 0;
    usize stm_idx       = static_cast<usize>(pos.active_color());
    update_hist_entry(m_main_hist[stm_idx][move.from_to()][from_attacked * 2 + to_attacked], bonus);

    PieceType pt     = pos.piece_at(move.from());
    usize     pt_idx = static_cast<usize>(pt) - static_cast<usize>(PieceType::Pawn);
    if (ply >= 1 && (ss - 1)->cont_hist_entry != nullptr) {
        update_hist_entry((*(ss - 1)->cont_hist_entry)[stm_idx][pt_idx][move.to().raw], bonus);
    }
    if (ply >= 2 && (ss - 2)->cont_hist_entry != nullptr) {
        update_hist_entry((*(ss - 2)->cont_hist_entry)[stm_idx][pt_idx][move.to().raw], bonus);
    }
}

void History::update_correction_history(Color side, HashKey pawn_key, i32 depth, i32 diff) {
    usize pawn_index  = static_cast<usize>(pawn_key % CORRECTION_HISTORY_ENTRY_NB);
    i32&  entry       = m_corr_hist[static_cast<usize>(side)][pawn_index];
    i32   new_weight  = std::min(16, 1 + depth);
    i32   scaled_diff = diff * CORRECTION_HISTORY_GRAIN;

    i32 update = entry * (CORRECTION_HISTORY_WEIGHT_SCALE - new_weight) + scaled_diff * new_weight;

    entry = std::clamp(update / CORRECTION_HISTORY_WEIGHT_SCALE, -CORRECTION_HISTORY_MAX,
                       CORRECTION_HISTORY_MAX);
}

i32 History::get_correction(Color side, HashKey pawn_key) {
    usize pawn_index = static_cast<usize>(pawn_key % CORRECTION_HISTORY_ENTRY_NB);
    return m_corr_hist[static_cast<usize>(side)][pawn_index] / CORRECTION_HISTORY_GRAIN;
}

void History::clear() {
    std::memset(&m_main_hist, 0, sizeof(MainHistory));
    std::memset(&m_cont_hist, 0, sizeof(ContHistory));
    std::memset(&m_corr_hist, 0, sizeof(CorrectionHistory));
}

}
