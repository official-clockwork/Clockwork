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
    case Stage::GenerateMoves:
        generate_moves();

        m_stage         = Stage::EmitTTMove;
        m_current_index = 0;

        [[fallthrough]];
    case Stage::EmitTTMove:
        m_stage = Stage::ScoreNoisy;
        if (m_tt_move != Move::none()) {
            return m_tt_move;
        }

        [[fallthrough]];
    case Stage::ScoreNoisy:
        score_moves(m_noisy);
        m_stage = Stage::EmitNoisy;

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

        m_stage         = Stage::EmitQuiet;
        m_current_index = 0;

        [[fallthrough]];
    case Stage::EmitQuiet:
        while (m_current_index < m_quiet.size()) {
            Move curr = m_quiet[m_current_index++];
            if (curr != m_tt_move) {
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
    for (Move mv : m_noisy) {
        if (mv == m_tt_move) {
            return;
        }
    }

    for (Move mv : m_quiet) {
        if (mv == m_tt_move) {
            return;
        }
    }
    // tt move is not legal
    m_tt_move = Move::none();
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
        // to be implemented
        return 0;
    } else {
        return 100 * static_cast<int>(m_pos.piece_at(move.to()))
             - static_cast<int>(m_pos.piece_at(move.from()));
    }
}

}
