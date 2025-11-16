#include "movepick.hpp"
#include "immintrin.h"
#include "see.hpp"
#include "tuned.hpp"
#include <emmintrin.h>
#include <immintrin.h>

namespace Clockwork {

bool quiet_move(Move move) {
    return !move.is_capture() && (!move.is_promotion() || move.promo().value() != PieceType::Queen);
}

void MovePicker::skip_quiets() {
    m_skip_quiets = true;
    if (m_stage == Stage::EmitQuiet) {
        m_current_index = 0;
        m_stage         = Stage::EmitBadNoisy;
    }
}
Move MovePicker::next() {
    switch (m_stage) {
    case Stage::EmitTTMove:
        m_stage = Stage::GenerateMoves;
        if (m_tt_move != Move::none() && m_movegen.is_legal(m_tt_move)) {
            return m_tt_move;
        }

        [[fallthrough]];
    case Stage::GenerateMoves:
        generate_moves();

        m_stage = Stage::ScoreNoisy;

        [[fallthrough]];
    case Stage::ScoreNoisy:
        score_moves(m_noisy);

        m_stage         = Stage::EmitGoodNoisy;
        m_current_index = 0;

        [[fallthrough]];
    case Stage::EmitGoodNoisy:
        while (m_current_index < m_noisy.size()) {
            auto [curr, score] = pick_next(m_noisy);
            // Check see
            if (curr != m_tt_move) {
                if (SEE::see(m_pos, curr, -score / tuned::movepicker_see_capthist_divisor)) {
                    return curr;
                } else {
                    m_bad_noisy.push_back(curr);
                }
            }
        }

        if (m_skip_quiets) {
            m_current_index = 0;
            m_stage         = Stage::EmitBadNoisy;
            goto emit_bad_noisy;
        }

        m_stage = Stage::EmitKiller;

        [[fallthrough]];

    case Stage::EmitKiller:
        m_stage = Stage::ScoreQuiet;
        if (m_tt_move != m_killer && m_killer != Move::none() && m_movegen.is_legal(m_killer)) {
            return m_killer;
        }

        [[fallthrough]];

    case Stage::ScoreQuiet:
        score_moves(m_quiet);

        m_stage         = Stage::EmitQuiet;
        m_current_index = 0;

        [[fallthrough]];
    case Stage::EmitQuiet:
        while (m_current_index < m_quiet.size()) {
            auto [curr, score] = pick_next(m_quiet);
            if (curr != m_tt_move && curr != m_killer) {
                return curr;
            }
        }

        // Reset the current index to 0 to start from the beginning of the noisy moves again.
        m_current_index = 0;
        m_stage         = Stage::EmitBadNoisy;

emit_bad_noisy:
        [[fallthrough]];
    case Stage::EmitBadNoisy:
        while (m_current_index < m_bad_noisy.size()) {
            Move curr = m_bad_noisy[m_current_index++];
            if (curr != m_tt_move && curr != m_killer) {
                return curr;
            }
        }
        m_stage = Stage::End;

        [[fallthrough]];
    case Stage::End:
        return Move::none();
    }
    unreachable();
}

void MovePicker::generate_moves() {
    m_movegen.generate_moves(m_noisy, m_quiet);
}

void MovePicker::score_moves(MoveList& moves) {
    for (usize i = 0; i < moves.size(); i++) {
        m_scores[i] = score_move(moves[i]);
    }
}

std::pair<Move, i32> MovePicker::pick_next(MoveList& moves) {
    u32x4 best_indices = u32x4::splat(static_cast<u32>(m_current_index));
    i32x4 best_values  = i32x4::splat(m_scores[m_current_index]);

    u32x4 indicies = best_indices + u32x4{{1, 2, 3, 4}};
    usize i        = m_current_index + 1;
    for (; i + 3 < moves.size(); i += 4, indicies += u32x4::splat(4)) {
        i32x4 values  = i32x4::load(&m_scores[i]);
        m32x4 greater = values.gt(best_values);

        best_values  = greater.select(best_values, values);
        best_indices = greater.select(best_indices, indicies);
    }

    std::array<u32, 4> indices_array = best_indices.to_array();
    std::array<i32, 4> values_array  = best_values.to_array();

    usize best_vectorized_index = 0;

    for (usize j = 1; j < 4; ++j) {
        if (values_array[j] > values_array[best_vectorized_index]) {
            best_vectorized_index = j;
        }
    }

    usize best_idx = indices_array[best_vectorized_index];

    for (; i < moves.size(); i++) {
        if (m_scores[i] > m_scores[best_idx]) {
            best_idx = i;
        }
    }

    std::swap(m_scores[m_current_index], m_scores[best_idx]);
    std::swap(moves[m_current_index], moves[best_idx]);
    return {moves[m_current_index], m_scores[m_current_index++]};
}

i32 MovePicker::score_move(Move move) const {
    if (quiet_move(move)) {
        return m_history.get_quiet_stats(m_pos, move, m_ply, m_stack);
    } else if (move.is_promotion()) {
        return 500 + m_history.get_noisy_stats(m_pos, move);
    } else {
        constexpr int MVV[6]   = {0, 800, 2400, 2400, 4800, 7200};
        PieceType     captured = move.is_en_passant() ? PieceType::Pawn : m_pos.piece_at(move.to());

        return MVV[static_cast<usize>(captured)] + m_history.get_noisy_stats(m_pos, move);
    }
}

}  // namespace Clockwork
