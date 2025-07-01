#include "movepick.hpp"

namespace Clockwork {

bool quiet_move(Move move) {
    return !move.is_capture() && (!move.is_promotion() || move.promo().value() != PieceType::Queen);
}

void MovePicker::skip_quiets() {
    m_skip_quiets = true;
    if (m_stage == Stage::EmitQuiet) {
        m_stage = Stage::End;
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

        m_stage         = Stage::EmitNoisy;
        m_current_index = 0;

        [[fallthrough]];
    case Stage::EmitNoisy:
        while (m_current_index < m_noisy.size()) {
            Move curr = pick_next(m_noisy);
            if (curr != m_tt_move) {
                return curr;
            }
        }

        if (m_skip_quiets) {
            m_stage = Stage::End;
            return Move::none();
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
            Move curr = pick_next(m_quiet);
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

Move MovePicker::pick_next(MoveList& moves) {
    u32 best_idx = m_current_index;
    for (u32 i = m_current_index + 1; i < moves.size(); i++) {
        if (m_scores[i] > m_scores[best_idx]) {
            best_idx = i;
        }
    }

    std::swap(m_scores[m_current_index], m_scores[best_idx]);
    std::swap(moves[m_current_index], moves[best_idx]);
    return moves[m_current_index++];
}

i32 MovePicker::score_move(Move move) const {
    if (quiet_move(move)) {
        return m_history.get_quiet_stats(m_pos, move);
    } else if (move.is_promotion()) {
        return 500;
    } else {
        return 100 * static_cast<int>(m_pos.piece_at(move.to()))
             - static_cast<int>(m_pos.piece_at(move.from()));
    }
}

}
