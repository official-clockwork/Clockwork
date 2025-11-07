#include "history.hpp"
#include "search.hpp"

namespace Clockwork {

i32 History::get_conthist(const Position& pos, Move move, i32 ply, Search::Stack* ss) const {
    i32 stats = 0;

    usize     stm_idx = static_cast<usize>(pos.active_color());
    PieceType pt      = pos.piece_at(move.from());
    usize     pt_idx  = static_cast<usize>(pt) - static_cast<usize>(PieceType::Pawn);
    if (ply >= 1 && (ss - 1)->cont_hist_entry != nullptr) {
        stats += (*(ss - 1)->cont_hist_entry)[stm_idx][pt_idx][move.to().raw];
    }
    if (ply >= 2 && (ss - 2)->cont_hist_entry != nullptr) {
        stats += (*(ss - 2)->cont_hist_entry)[stm_idx][pt_idx][move.to().raw];
    }
    if (ply >= 4 && (ss - 4)->cont_hist_entry != nullptr) {
        stats += (*(ss - 4)->cont_hist_entry)[stm_idx][pt_idx][move.to().raw];
    }
    if (ply >= 6 && (ss - 6)->cont_hist_entry != nullptr) {
        stats += (*(ss - 6)->cont_hist_entry)[stm_idx][pt_idx][move.to().raw];
    }

    return stats;
}

i32 History::get_quiet_stats(const Position& pos, Move move, i32 ply, Search::Stack* ss) const {
    auto to_attacked   = pos.is_square_attacked_by(move.to(), ~pos.active_color());
    auto from_attacked = pos.is_square_attacked_by(move.from(), ~pos.active_color());
    i32  stats         = m_main_hist[static_cast<usize>(pos.active_color())][move.from_to()]
                           [from_attacked * 2 + to_attacked];
    stats += 2 * get_conthist(pos, move, ply, ss);
    return stats;
}

void History::update_cont_hist(
  const Position& pos, Move move, i32 ply, Search::Stack* ss, i32 bonus) {
    i32       conthist = get_conthist(pos, move, ply, ss);
    PieceType pt       = pos.piece_at(move.from());
    usize     pt_idx   = static_cast<usize>(pt) - static_cast<usize>(PieceType::Pawn);
    usize     stm_idx  = static_cast<usize>(pos.active_color());
    if (ply >= 1 && (ss - 1)->cont_hist_entry != nullptr) {
        update_hist_entry_banger((*(ss - 1)->cont_hist_entry)[stm_idx][pt_idx][move.to().raw],
                                 conthist, bonus);
    }
    if (ply >= 2 && (ss - 2)->cont_hist_entry != nullptr) {
        update_hist_entry_banger((*(ss - 2)->cont_hist_entry)[stm_idx][pt_idx][move.to().raw],
                                 conthist, bonus);
    }
    if (ply >= 4 && (ss - 4)->cont_hist_entry != nullptr) {
        update_hist_entry_banger((*(ss - 4)->cont_hist_entry)[stm_idx][pt_idx][move.to().raw],
                                 conthist, bonus);
    }
    if (ply >= 6 && (ss - 6)->cont_hist_entry != nullptr) {
        update_hist_entry_banger((*(ss - 6)->cont_hist_entry)[stm_idx][pt_idx][move.to().raw],
                                 conthist, bonus);
    }
}


void History::update_quiet_stats(
  const Position& pos, Move move, i32 ply, Search::Stack* ss, i32 bonus) {
    auto  to_attacked   = pos.is_square_attacked_by(move.to(), ~pos.active_color());
    auto  from_attacked = pos.is_square_attacked_by(move.from(), ~pos.active_color());
    usize stm_idx       = static_cast<usize>(pos.active_color());
    update_hist_entry(m_main_hist[stm_idx][move.from_to()][from_attacked * 2 + to_attacked], bonus);
    update_cont_hist(pos, move, ply, ss, bonus);
}

i32 History::get_noisy_stats(const Position& pos, Move move) const {
    usize     stm_idx  = static_cast<usize>(pos.active_color());
    PieceType pt       = pos.piece_at(move.from());
    usize     pt_idx   = static_cast<usize>(pt) - static_cast<usize>(PieceType::Pawn);
    PieceType captured = move.is_en_passant() ? PieceType::Pawn : pos.piece_at(move.to());
    return m_capt_hist[stm_idx][pt_idx][static_cast<usize>(captured)][move.to().raw];
}

void History::update_noisy_stats(const Position& pos, Move move, i32 bonus) {
    usize     stm_idx  = static_cast<usize>(pos.active_color());
    PieceType pt       = pos.piece_at(move.from());
    usize     pt_idx   = static_cast<usize>(pt) - static_cast<usize>(PieceType::Pawn);
    PieceType captured = move.is_en_passant() ? PieceType::Pawn : pos.piece_at(move.to());
    update_hist_entry(m_capt_hist[stm_idx][pt_idx][static_cast<usize>(captured)][move.to().raw],
                      bonus);
}

void History::update_correction_history(const Position& pos, i32 depth, i32 diff) {
    usize side_index         = static_cast<usize>(pos.active_color());
    u64   pawn_key           = pos.get_pawn_key();
    u64   white_non_pawn_key = pos.get_non_pawn_key(Color::White);
    u64   black_non_pawn_key = pos.get_non_pawn_key(Color::Black);
    u64   major_key          = pos.get_major_key();
    u64   minor_key          = pos.get_minor_key();
    usize pawn_index         = static_cast<usize>(pawn_key % CORRECTION_HISTORY_ENTRY_NB);
    usize white_non_pawn_index =
      static_cast<usize>(white_non_pawn_key % CORRECTION_HISTORY_ENTRY_NB);
    usize black_non_pawn_index =
      static_cast<usize>(black_non_pawn_key % CORRECTION_HISTORY_ENTRY_NB);
    usize major_index = static_cast<usize>(major_key % CORRECTION_HISTORY_ENTRY_NB);
    usize minor_index = static_cast<usize>(minor_key % CORRECTION_HISTORY_ENTRY_NB);

    i32 new_weight  = std::min(16, 1 + depth);
    i32 scaled_diff = diff * CORRECTION_HISTORY_GRAIN;

    auto update_entry = [=](i32& entry) {
        i32 update =
          entry * (CORRECTION_HISTORY_WEIGHT_SCALE - new_weight) + scaled_diff * new_weight;

        entry = std::clamp(update / CORRECTION_HISTORY_WEIGHT_SCALE, -CORRECTION_HISTORY_MAX,
                           CORRECTION_HISTORY_MAX);
    };

    update_entry(m_pawn_corr_hist[side_index][pawn_index]);
    update_entry(m_non_pawn_corr_hist[0][side_index][white_non_pawn_index]);
    update_entry(m_non_pawn_corr_hist[1][side_index][black_non_pawn_index]);
    update_entry(m_major_corr_hist[side_index][major_index]);
    update_entry(m_minor_corr_hist[side_index][minor_index]);
}

i32 History::get_correction(const Position& pos) {
    usize side_index         = static_cast<usize>(pos.active_color());
    u64   pawn_key           = pos.get_pawn_key();
    u64   white_non_pawn_key = pos.get_non_pawn_key(Color::White);
    u64   black_non_pawn_key = pos.get_non_pawn_key(Color::Black);
    u64   major_key          = pos.get_major_key();
    u64   minor_key          = pos.get_minor_key();
    usize pawn_index         = static_cast<usize>(pawn_key % CORRECTION_HISTORY_ENTRY_NB);
    usize white_non_pawn_index =
      static_cast<usize>(white_non_pawn_key % CORRECTION_HISTORY_ENTRY_NB);
    usize black_non_pawn_index =
      static_cast<usize>(black_non_pawn_key % CORRECTION_HISTORY_ENTRY_NB);
    usize major_index = static_cast<usize>(major_key % CORRECTION_HISTORY_ENTRY_NB);
    usize minor_index = static_cast<usize>(minor_key % CORRECTION_HISTORY_ENTRY_NB);

    i32 correction = 0;
    correction += m_pawn_corr_hist[side_index][pawn_index];
    correction += m_non_pawn_corr_hist[0][side_index][white_non_pawn_index];
    correction += m_non_pawn_corr_hist[1][side_index][black_non_pawn_index];
    correction += m_major_corr_hist[side_index][major_index];
    correction += m_minor_corr_hist[side_index][minor_index];

    return correction / CORRECTION_HISTORY_GRAIN;
}

void History::clear() {
    std::memset(&m_main_hist, 0, sizeof(MainHistory));
    std::memset(&m_cont_hist, 0, sizeof(ContHistory));
    std::memset(&m_capt_hist, 0, sizeof(CaptHistory));
    std::memset(&m_pawn_corr_hist, 0, sizeof(CorrectionHistory));
    std::memset(&m_non_pawn_corr_hist[0], 0, sizeof(CorrectionHistory));
    std::memset(&m_non_pawn_corr_hist[1], 0, sizeof(CorrectionHistory));
    std::memset(&m_major_corr_hist, 0, sizeof(CorrectionHistory));
    std::memset(&m_minor_corr_hist, 0, sizeof(CorrectionHistory));
}

}  // namespace Clockwork
