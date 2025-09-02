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

void History::clear() {
    std::memset(&m_main_hist, 0, sizeof(MainHistory));
    std::memset(&m_cont_hist, 0, sizeof(ContHistory));
}
}
