#include "movepick.hpp"

namespace Clockwork {

void MovePicker::skip_quiets() {
    m_skip_quiets = true;
    if (m_stage == Stage::emit_quiet) {
        m_stage = Stage::end;
    }
}

Move MovePicker::next() {
    switch (m_stage) {
    case Stage::generate_moves:
        generate_moves();

        m_stage         = Stage::emit_noisy;
        m_current_index = 0;

        [[fallthrough]];
    case Stage::emit_noisy:
        if (m_current_index < m_noisy.size()) {
            return m_noisy[m_current_index++];
        }

        if (m_skip_quiets) {
            m_stage = Stage::end;
            return Move::none();
        }

        m_stage         = Stage::emit_quiet;
        m_current_index = 0;

        [[fallthrough]];
    case Stage::emit_quiet:
        if (m_current_index < m_quiet.size()) {
            return m_quiet[m_current_index++];
        }

        m_stage = Stage::end;

        [[fallthrough]];
    case Stage::end:
    default:
        return Move::none();
    }
}

void MovePicker::generate_moves() {
    m_movegen.generate_moves(m_noisy, m_quiet);
}

}
