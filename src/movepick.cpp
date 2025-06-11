#include "movepick.hpp"

namespace Clockwork {

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
        m_stage = Stage::EmitNoisy;
        if (m_tt_move != Move::none()) {
            return m_tt_move;
        }

        [[fallthrough]];
    case Stage::EmitNoisy:
        while (m_current_index < m_noisy.size()) {
            Move curr = m_noisy[m_current_index++];
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

}
